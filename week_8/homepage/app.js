// alert("HELLO FROM APP.JS!");

var posters = [
    {img:"images/captain-fantastic.jpg",title:"Captain Fantastic"},
    {img:"images/before-sunrise.jpg",title:"Before Sunrise"},
    {img:"images/before-sunset.jpg",title:"Before Sunset"},
    {img:"images/before-midnight.jpg",title:"Before Midnight"},
    {img:"images/into-the-wild.jpg",title:"Into the Wild"},
    {img:"images/500-days-of-summer.jpg",title:"500 Days of Summer"}
];

var pets = [
    {img:"images/pet-eros.jpg",name:"Eros"},
    {img:"images/pet-chuva.jpg",name:"Chuva"},
    {img:"images/pet-gaia.jpg",name:"Gaia"}
];

var btn = document.querySelector("button");
var img = document.querySelector("#movie-poster"); 
var movie_title = document.querySelector("#movie-text");
var pet = document.querySelector("#pet-poster"); 
var pet_name = document.querySelector("#pet-text");
var counter = 1;

btn.addEventListener("click", function() {
    img.src = posters[counter].img
    movie_title.innerHTML = posters[counter].title
    counter++;
    if (counter == 6) {
        counter = 0
    }
});

btn.addEventListener("click", function() {
    pet.src = pets[counter].img
    pet_name.innerHTML = pets[counter].name
    counter++;
    if (counter == 3) {
        counter = 0
    }
});