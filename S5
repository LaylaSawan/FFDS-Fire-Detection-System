const mysql = require('mysql'); //bringing in the mysql library

const con = mysql.createConnection({ //creating the connection with mysql dtaabase
    host: process.env.RDS_HOSTNAME,
    user: process.env.RDS_USERNAME,
    password: process.env.RDS_PASSWORD,
    port: process.env.RDS_PORT,
    database: process.env.RDS_DATABASE
});

exports.handler = (event, context, callback) => {
    context.callbackWaitsForEmptyEventLoop = false;

    // Check if queryStringParameters is present
    
    const queryStringParameters = event.queryStringParameters;

    // Get packet from querystringparameters
    const myArray = queryStringParameters['packet'].split(",");

    console.log(myArray) //printing of the array


    console.log(queryStringParameters) //printing off queryStringParameters
    if (queryStringParameters) {
        // INSERT INTO THE DATABASE....
        
        const sql = "INSERT INTO `sensor_data` (node, co, co2, flame, temp, humidity) VALUES (?, ?, ?, ?, ?, ?)";
        const values = [
           myArray[0], // node value (first elementof the arrau) 
            myArray[1], // mq135 value (secnd element of the array) /* ask waleed if I should delete this from the array */
            myArray[2], // third value
            myArray[3], // fourth value
            myArray[4], //temperature
            myArray[5],
        ];

        con.query(sql, values, (err, res) => { //sending back that their is an error in the code or the database
            if (err) {
                console.error(err);
                callback(err, null);
            } else {
                console.log('1 record inserted.');
                callback(null, '1 record inserted.'); // inseting into the database
            }
        });
    }
};

// con.query(sql, function (err, result, fields) {
//     //       if (err) {
//     //         console.error(err);
//     //         callback(err);
//     //       } else {
//     //         console.log(result);
//     //         callback(null, result); // Assuming you want to return the result
//     //       }
//     //     });
//     //   });
