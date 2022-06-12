function checkQuestionOne(foo, id) {
    if (foo)
    {
        document.querySelector("#" + id).style.background = "green";
        document.querySelector("#result1").innerHTML = "Correct!"
    }
    else
    {
        if (document.querySelector("#result1").innerHTML != "Correct!")
        {
            document.querySelector("#" + id).style.background = "red";
            document.querySelector("#result1").innerHTML = "Incorrect"
        }
    }
};

function checkQuestionTwo(text) {
    if (text.toLowerCase() == "i solemnly swear that i am up to no good") {
        document.querySelector("#answer").style.backgroundColor = "green";
        document.querySelector("#result2").innerHTML = "Correct!"
    }
    else {
        document.querySelector("#answer").style.backgroundColor = "red";
        document.querySelector("#result2").innerHTML = "Incorrect"
    }
};

