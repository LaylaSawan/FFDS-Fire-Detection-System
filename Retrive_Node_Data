const mysql = require('mysql'); // getting the mysql linrary

const con = mysql.createConnection({ // creating a mysql connection
    host: process.env.RDS_HOSTNAME,
    user: process.env.RDS_USERNAME,
    password: process.env.RDS_PASSWORD,
    port: process.env.RDS_PORT,
    database: process.env.RDS_DATABASE
});

exports.handler = (event, context, callback) => {
    context.callbackWaitsForEmptyEventLoop = false;
    const node_query = 'SELECT * FROM mydatabase.node_data'; // seletcin everything from node data the table
    con.query(node_query, (nodeerr, noderes) => {
        if (nodeerr) {
            console.log(nodeerr);
            throw nodeerr
        }
        let promises = [];
        for (let idx = 0; idx < noderes.length; idx++) {
            const sensor_query = `SELECT * FROM sensor_data WHERE node= ${noderes[idx].node} ORDER BY date_time DESC LIMIT 25;` // grabbing only 25 rows from sensor data
            console.log(sensor_query);
            promises.push(new Promise((resolve, reject) => {
                con.query(sensor_query, (sensorerr, sensorres) => {
                    if (sensorerr) {
                        console.error(sensorerr);
                        reject(sensorerr);
                    } else {
                        console.log(sensorres);
                        noderes[idx].sensorData = sensorres; // Add sensor data to node object
                        resolve();
                    }
                });
            }));
        };
        Promise.all(promises)
            .then(() => {
                callback(null, noderes); // Return nodes with sensor data
            })
            .catch((error) => {
                callback(error);
            });
    });
};
