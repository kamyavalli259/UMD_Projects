"use strict";

let whatIndex = 0;
let photos = ["InitialImage.jpg"];/*store the photo names */
let image = document.querySelector("#img");
let form = document.querySelector("#Photo_form");

let firstPhoto= document.querySelector("#button3");
let lastPhoto = document.querySelector("#button4");
let previousPhoto = document.querySelector("#button1");
let nextPhoto = document.querySelector("#button2");

let slideShow = document.querySelector("#slideshow1");
let randomShow = document.querySelector("#slideshow2");
let stopShow = document.querySelector("#slideshow3");
let interval;

let display = document.querySelector("#display");

let loaded = false;

let jsonImage = document.querySelector("#Loader2");
let reset = document.querySelector("#reset")


form.addEventListener("submit", (event)=>{
    

    event.preventDefault();
   
    /*when the submit event occurs*/
    let folder = document.querySelector("#folder").value;
    let common = document.querySelector("#common").value;
    let photo_number = document.querySelector("#photo_number").value;
    let end_of_photo_number= document.querySelector("#end_photo_number").value;
    if(end_of_photo_number< photo_number){
      document.querySelector("#red_text").textContent = "Error: Invalid Range";
      return;
    }
    loaded=true;
    photos=[];
    

    /*start to end and photo names to photos[] */
    for(let i = photo_number; i<= end_of_photo_number; i++){
        photos.push(folder+common+i+".jpg");
    }
    /*getElement by id it is taking the html element that have that id and change it */
    const image = document.getElementById("img");
    image.src = photos[0];
    
    console.log(photos);
    document.querySelector("#red_text").textContent="Photo Viewer System";

})

firstPhoto.addEventListener("click", ()=>{
    image.src=photos[0];
    whatIndex = 0;
    display.value=photos[whatIndex];
});

lastPhoto.addEventListener("click", ()=>{
    image.src= photos[photos.length-1];
    whatIndex = photos.length-1;
    display.value=photos[whatIndex];
});

nextPhoto.addEventListener("click", ()=>{
    if(loaded==false){
        document.querySelector("#red_text").textContent="Error: you must load data first";
        return 
    }
    whatIndex= whatIndex+1;
    if(whatIndex>=photos.length){
        whatIndex = 0;
    }
    image.src=photos[whatIndex];
    display.value=photos[whatIndex];
})

previousPhoto.addEventListener("click", ()=>{
    if(loaded==false){
        document.querySelector("#red_text").textContent="Error: you must load data first";
        return 
    }
    whatIndex = whatIndex-1;
    if(whatIndex<0){
        whatIndex = photos.length-1;
    }
    image.src=photos[whatIndex];
    display.value=photos[whatIndex];
})

slideShow.addEventListener("click", ()=>{
    if(loaded==false){
        document.querySelector("#red_text").textContent="Error: you must load data first";
        return 
    }
    clearInterval(interval);/*starts new and no weird behavior*/
    interval=setInterval(()=>{
    whatIndex= whatIndex+1;
    if(whatIndex>=photos.length){
        whatIndex = 0;
    }
    image.src=photos[whatIndex];
    display.value=photos[whatIndex];
    },1000);
})

randomShow.addEventListener("click", ()=>{
    if(loaded==false){
        document.querySelector("#red_text").textContent="Error: you must load data first";
        return 
    }
    clearInterval(interval);/*starts new and no weird behavior*/
    interval=setInterval(()=>{
    whatIndex= Math.floor(Math.random()*photos.length);
    if(whatIndex>=photos.length){
        whatIndex = 0;
    }
    image.src=photos[whatIndex];
    display.value=photos[whatIndex];
    },1000);
})

stopShow.addEventListener("click", ()=>{
    clearInterval(interval);
})

image.addEventListener("click", ()=>{
    whatIndex= whatIndex+1;
    if(whatIndex>=photos.length){
        whatIndex = 0;
    }
    image.src=photos[whatIndex];
    display.value = photos[whatIndex];
})

jsonImage.addEventListener("click", ()=>{
    let jsonFile = document.querySelector("#load_image").value;
    fetch(jsonFile).then(response=>{
        if(!response.ok){
            throw new Error("Error: Loading JSON");
        }
        
        return response.json();
    }).then(data=>{
        console.log(data);
        photos=[]
        for(let p of data.images){
            photos.push(p.imageURL);
        }
        const image = document.getElementById("img");
        image.src = photos[0];
        // display.value=photos[whatIndex];
        loaded=true;

    }).catch(error=>{
        console.log("Error");
    })
})


reset.addEventListener("click", ()=>{
    document.querySelector("#folder").value="umcp/";
    document.querySelector("#common").value = "college";
    document.querySelector("#photo_number").value = "2";
    document.querySelector("#end_photo_number").value="4";
    document.querySelector("#load_image").value="http://www.cs.umd.edu/~nelson/classes/resources/cmsc335/images/imagesSet1.json";
    document.querySelector("#display").value="";

})


