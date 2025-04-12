const http = require('http');
const fs = require('fs');
const express = require('express');
const app = express();
const path = require('path');
const bodyParser = require('body-parser');

process.stdin.setEncoding("utf8");
const portNumber = process.argv[2];

require("dotenv").config();

const { MongoClient, ServerApiVersion } = require('mongodb');
const userName = process.env.MONGO_DB_USERNAME;
const password = process.env.MONGO_DB_PASSWORD;
const db = process.env.MONGO_DB_NAME;
const collect = process.env.MONGO_COLLECTION;

const databaseAndCollection = {
    db: db,
    collection: collect,
};

/*const uri = "mongodb+srv://"+userName+":"+password+"@cluster0.hb5gb.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";*/
/*const uri = "mongodb+srv://" + userName + ":" + password + "@cluster0.5uvgyon.mongodb.net/?retryWrites=true&w=majority";*/
/*const uri = "mongodb+srv://"+userName+":"+password+"@cluster0.pnsre.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";*/
const uri = "mongodb+srv://"+userName+":"+password+"@cluster0.9ejnw.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";


const client = new MongoClient(uri, {serverApi: ServerApiVersion.v1, autoSelectFamily:false});
let database; 

if (process.argv.length!=3){
    process.stdout.write(`Usage: node summerCampServer.js portNumber\n`);
    process.exit(1);
}
client.connect().then(()=>{
    database = client.db("campApplicants");
    /*console.log("connected to database\n");*/
    app.listen(portNumber);
    console.log(`Web server started and running at http://localhost:${portNumber}`);
    process.stdout.write(`Stop to shutdown the server: `);
})
.catch(err=>console.log("failed to connect", err));


process.stdin.on('readable', ()=>{
    let inputData = process.stdin.read();
    if(inputData !== null){
        let command = inputData.toString().trim();
        if (command === "stop") {
            process.stdout.write("Shutting down the server\n")
            process.exit(0);
        } 
    }
});


app.set("views", path.resolve(__dirname, "templates"));
app.set("view engine", "ejs");
app.use(bodyParser.urlencoded({extended:false}));

app.get("/", (req,res)=>{
    res.render("index");
});
app.get("/apply", (req, res)=>{
    res.render("appProcess", {name:"", email:"", gpa:"", information:""});
});

async function app_submit(newForm){
    /*console.log(newForm);*/
    /*await client.connect();
        await client.db(databaseAndCollection).collection(databaseAndCollection.collection).insertOne(newForm);*/
        const result = await client.db(databaseAndCollection.db).collection(databaseAndCollection.collection);
        const collection = database.collection(databaseAndCollection.collection);
    try{
        await result.insertOne(newForm);
    }catch(error){
        console.log(error);
    }
}
app.post("/apply", async(req, res)=>{
    /*console.log(req.body);*/
    const{name, email, gpa, info}= req.body;
    let newForm = {name: name, email: email, gpa:gpa, info:info};
    app_submit(newForm)
    const time = new Date().toString();
    res.render("appReview", {name, email, gpa, info, time});
});


app.get("/reviewApplication", (req, res)=>{
    res.render("reviewApplication", {portNumber});
});
async function get_application(query){
    const collection = database.collection(databaseAndCollection.collection);
    const result = await client.db(databaseAndCollection.db).collection(databaseAndCollection.collection);
    try{
        const col = await result.find(query).toArray();
        return col;
    }catch(error){
        console.log(error);
    }
}
app.post("/processReviewApplication", async(req, res)=>{
    const{email}= req.body;
    let newForm = {email: email}
    /*console.log(newForm);*/
    get_application(newForm).then((results)=>{
        /*console.log(results);*/
    
    const name = results[0].name;
    const gpa = results[0].gpa;
    const info = results[0].info;
    const time = new Date().toString();
    res.render("appReview", {name, email, gpa, info, time});
    });
});


app.get("/adminGFA", (req, res)=>{
    res.render("adminGFA", {portNumber});
})

async function get_gpa(minGPA){
    const result = client.db(databaseAndCollection.db).collection(databaseAndCollection.collection);
    const collection = database.collection(databaseAndCollection.collection);
    try{
        const col = await result.find({gpa: {$gte:minGPA}}).toArray();
        return col;
    }catch(error){
        console.log(error);
    }
}
app.post("/processAdminGFA", async(req, res)=>{
    const{gpa}= req.body;
    let table = "<table border = 1><tr><th>Name</th><th>GPA</th></tr>"
    let r =  await get_gpa(gpa);
    r.forEach((elem => {
        table += `<tr><td>${elem.name}</td><td>${elem.gpa}</td></tr>`;
    }))
    table += "</table>";
    
    res.render("displayGPA", {table: table});
});


app.get("/adminRemove", (req, res) => {
    const url = "http://localhost:"+portNumber+"/adminRemove";
    res.render("adminRemove", {url});
  });
  

  /*Route to handle the remove-all request*/
    app.post("/adminRemove", async(req, res) => {
        try {
           
          await client.connect();
          const result = await client.db(databaseAndCollection.db).collection(databaseAndCollection.collection).deleteMany({});
          const count = result.deletedCount;
      
          // Render confirmation page
          res.render("appRemove", { count });
        } catch (e) {
          console.error(e);
          res.status(500).send("An error occurred while removing applications.");
        } 
      });
