import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    stocks = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session.get("user_id"))
    user_cash = db.execute("SELECT cash from users WHERE id = ?", session.get("user_id"))[0]['cash']

    # return render_template("test.html", data=stocks)

    for item in stocks:
        item["symbol"] = item["symbol"].upper()
        item["name"] = lookup(item["symbol"])["name"]
        item["price"] = usd(lookup(item["symbol"])["price"])
        item["total"] = usd(lookup(item["symbol"])["price"] * item["shares"])

    account_total = user_cash

    for item in stocks:
        account_total += lookup(item["symbol"])["price"] * item["shares"]

    return render_template("index.html", stocks=stocks, user_cash=usd(user_cash), account_total=usd(account_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Quantity must be a positive integer")

        # Handle errors
        if not symbol:
            return apology("Must provide symbol")
        elif not lookup(symbol):
            return apology("Symbol do not exist")
        elif not shares:
            return apology("Quantity must be a positive integer")
        elif int(shares) < 0:
            return apology("Quantity must be a positive integer")

        # Transaction
        symbol_price = lookup(symbol)["price"]  # Check stock unit price
        user_cash = db.execute("SELECT cash from users WHERE id = ?", session.get("user_id"))[
            0]['cash']  # Check how much cash the user has
        total = symbol_price * shares
        final_balance = user_cash - total
        if final_balance < 0:
            return apology("Not enough funds")

        # Update database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", final_balance, session.get("user_id"))  # Update user account balance
        db.execute("INSERT INTO transactions (user_id, transaction_datetime, transaction_type, symbol, shares, share_price, total) VALUES (?, datetime('now'), 'buy', ?, ?, ?, ?)",
                   session.get("user_id"), symbol.upper(), shares, symbol_price, total)

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT UPPER(symbol) AS symbol, shares, share_price, transaction_datetime FROM transactions")

    for item in transactions:
        item["share_price"] = usd(item["share_price"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Must provide a symbol")
        elif lookup(request.form.get("symbol")) == None:
            return apology("Must provide a valid symbol")

        res = lookup(request.form.get("symbol"))
        quote_symbol = res["symbol"]
        quote_name = res["name"]
        quote_price = usd(res["price"])
        return render_template("quoted.html", quote_symbol=quote_symbol, quote_name=quote_name, quote_price=quote_price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Saving inputs as variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        hash = generate_password_hash(request.form.get("password"))

        # Ensure username is not blank
        if not username:
            return apology("must provide username", 400)

        # Ensure username is new
        if db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("user already exists", 400)

        # Ensure password is not blank
        if not password:
            return apology("must provide password", 400)

        # Ensure password and confirmation match
        if password != confirmation:
            return apology("passwords do not match", 400)

        # Insert the user credentials into the database and redirect to login page
        db.execute("INSERT INTO users (username, hash) VALUES (?,?)", username, hash)
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_stocks = db.execute(
        "SELECT UPPER(symbol) AS symbol FROM balance WHERE user_id = ? AND current_shares > 0", session.get("user_id"))
    stock_list = []
    for item in user_stocks:
        stock_list.append(item["symbol"].lower())

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Provide a symbol")
        if request.form.get("symbol").lower() not in stock_list:
            return apology("Provide a valid symbol")
        # if not lookup(request.form.get("symbol").lower()):
        if not lookup(request.form.get("symbol")):
            return apology("Symbol do not exist")

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Shares must be a positive integer")
        if shares < 1:
            return apology("Shares must be a positive integer")

        # symbol = request.form.get("symbol").lower()
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Transaction
        symbol_price = lookup(symbol)["price"]  # Check stock unit price

        # Check how much shares the user has for that symbol
        current_shares = db.execute("SELECT current_shares FROM balance WHERE user_id = ? AND symbol = ?",
                                    session.get("user_id"), symbol)
        if not current_shares:
            return apology("Not enough shares")
        current_shares = current_shares[0]['current_shares']

        if current_shares < shares:
            return apology("Not enough shares", 400)

        user_cash = float(db.execute("SELECT cash FROM users WHERE id = ?", session.get("user_id"))
                          [0]['cash'])  # Check how much cash the user has
        total = float(symbol_price) * shares
        final_balance = user_cash + total

        # Update database
        db.execute("UPDATE users SET cash = ? WHERE id = ?", final_balance, session.get("user_id"))  # Update user account balance
        db.execute("INSERT INTO transactions (user_id, transaction_datetime, transaction_type, symbol, shares, share_price, total) VALUES (?, datetime('now'), 'sell', ?, ?, ?, ?)",
                   session.get("user_id"), symbol, -shares, symbol_price, total)

        return redirect("/")

    else:
        return render_template("sell.html", data=user_stocks)
