
<?php
session_start();
require_once './MeraniaController.php';
$meraniaController = new MeraniaController();


//$createMeranie = $meraniaController->createMeranie($json_data[0]['node'],$json_data[0]['temp'], $json_data[0]['alt'],$json_data[0]['pres']);

//$createMeranie = $meraniaController->createMeranie(1,33.0,33.0,33.0);
?>

<!doctype html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js"></script>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.14.7/dist/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.3.1/dist/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/apexcharts"></script>
    <link href="styles.css" rel="stylesheet">
    <title>App</title>
</head>
<body>

<section class="text-center my-4">




    <h1>Nodes Monitoring</h1>
    <table class="table" id = "table">
    </table>




<!--    <p>-->
<!--        --><?php
//        require_once './MeraniaController.php';
//        $meraniaController = new MeraniaController();
//
//        $merania =  $meraniaController->getMeraniaForUser(1);
//
//        $pole = [];
//        forEach($merania as $meranie){
//            array_push($pole,  $meranie['id'], $meranie['temp']);
//        }
//
//        echo json_encode($pole);
//
////        echo json_encode($merania);
//
//        ?>
<!--    </p>-->
    <h3>Temperature</h3>
    <div id="chart">
    </div>
    <h3>Altitude</h3>
    <div id="chartAlt">
    </div>
    <h3>Pressure</h3>
    <div id="chartPressure">
    </div>
</section>





</body>


<script>
    const table = document.querySelector("#table");
    let selectedNode = 1;

    const addRow = () => {


        while (table.firstChild) {
            table.removeChild(table.firstChild);
        }

        const thead = document.createElement('thead');
        const tr = document.createElement('tr');
        const th0 = document.createElement('th');
        th0.innerText = " ";
        const th1 = document.createElement('th');
        th1.innerText = "Node Number";
        const th2 = document.createElement('th');
        th2.innerText = "Pressure";
        const th3 = document.createElement('th');
        th3.innerText = "Temperature";
        const th4 = document.createElement('th');
        th4.innerText = "Altitude";
        const th5 = document.createElement('th');
        th5.innerText = "Online";


        tr.append(th0,th1, th2,th3,th4,th5);
        thead.append(tr);
        table.append(thead);



        fetch("./data.php")
            .then(response => response.json())
            .then(data => {
                console.log(data);


                for (let d of data) {

                    let tr = document.createElement("tr");
                    let pictureTd= document.createElement("td");
                    let picture = document.createElement("img");
                    picture.src = 'img/node.png';
                    picture.style.height = "140px";
                    pictureTd.append(picture);
                    let nodeNumber = document.createElement("td");
                    let Pressure = document.createElement("td");
                    let Temperature = document.createElement("td");
                    let Altitude = document.createElement("td");
                    let Online = document.createElement("td");





                    nodeNumber.innerText = d['node'];
                    Pressure.innerText = d['pres'];
                    Temperature.innerText = d['temp'];
                    Altitude.innerText = d['alt'];
                    Online.innerText = d['online'];

                    d['online'] ? Online.style.color = "green" :  Online.style.color = "red";

                    tr.append(pictureTd,nodeNumber, Pressure, Temperature, Altitude, Online);



                    tr.addEventListener("mouseover", () => {


                        let trs = document.querySelectorAll("tr");
                        trs.forEach(tr => tr.style = "color:black");
                        tr.style.backgroundColor = "#FFFFE0";


                    });


                    tr.addEventListener("click", () => {
                        selectedNode = d['node'];

                        fetch("./updateChartData.php?node_id="+d['node'])
                            .then(response => response.json())
                            .then(data => {


                                chart.updateSeries([{
                                    data: data
                                }])



                            });

                        fetch("./updateAltChart.php?node_id="+d['node'])
                            .then(response => response.json())
                            .then(data => {


                                chartAlt.updateSeries([{
                                    data: data
                                }])



                            });
                        fetch("./updatePressureChart.php?node_id="+d['node'])
                            .then(response => response.json())
                            .then(data => {


                                chartPressure.updateSeries([{
                                    data: data
                                }])



                            });






                    });




                    table.append(tr);


                }

            })

    }



    var options = {
        chart: {
            height: 280,
            type: "area",
            
        },
        dataLabels: {

            enabled: false
        },
        series: [
            {
                name: "Temperature",
                data: [ ]
            }
        ],

        fill: {
            type: "gradient",
            gradient: {
                shadeIntensity: 1,
                opacityFrom: 0.7,
                opacityTo: 0.9,
                stops: [0, 90, 100]
            }
        },



    };
    var optionsAlt = {
        chart: {
            height: 280,
            type: "area",

        },
        dataLabels: {
            enabled: false
        },
        series: [
            {
                name: "Altitude",
                data: [ ]
            }
        ],
        colors: ["#FF1414"],
        fill: {
            type: "gradient",
            gradient: {
                shadeIntensity: 1,
                opacityFrom: 0.7,
                opacityTo: 0.9,
                stops: [0, 90, 100]
            }
        },

    };
    var optionsPressure = {
        chart: {
            height: 280,
            type: "area",

        },
        dataLabels: {
            enabled: false
        },
        series: [
            {
                name: "Pressure",
                data: [ ]
            }
        ],
        colors: ["#FFFF00"],
        fill: {
            type: "gradient",
            gradient: {
                shadeIntensity: 1,
                opacityFrom: 0.7,
                opacityTo: 0.9,
                stops: [0, 90, 100]
            }
        },

    };

    var chart = new ApexCharts(document.querySelector("#chart"), options);
    var chartAlt = new ApexCharts(document.querySelector("#chartAlt"), optionsAlt);
    var chartPressure = new ApexCharts(document.querySelector("#chartPressure"), optionsPressure);
    chart.render();
    chartAlt.render();
    chartPressure.render();







    const updateRow = () => {


        fetch("./updateChartData.php?node_id="+selectedNode)
            .then(response => response.json())
            .then(data => {

                console.log(data);
                chart.updateSeries([{
                    data: data
                }])



            });

        fetch("./updateAltChart.php?node_id="+selectedNode)
            .then(response => response.json())
            .then(data => {


                chartAlt.updateSeries([{
                    data: data
                }])



            });
        fetch("./updatePressureChart.php?node_id="+selectedNode)
            .then(response => response.json())
            .then(data => {


                chartPressure.updateSeries([{
                    data: data
                }])



            });


        fetch("./data.php")
            .then(response => response.json())
            .then(data => {

                for (let i = 1; i < table.rows.length; i++) {

                    let row = table.rows[i];

                    // console.log(i);
                    //iterate through rows
                    // console.log(data[i]['node']);
                    //rows would be accessed using the "row" variable assigned in the for loop

                    row.cells[1].innerText = data[i-1]['node']; //node Number
                    row.cells[2].innerText = data[i-1]['pres'];//node pressure
                    row.cells[3].innerText = data[i-1]['temp'];//node temp
                    row.cells[4].innerText = data[i-1]['alt'];//node altitude
                    row.cells[5].innerText = data[i-1]['online'];//node online





                    data[i-1]['online'] ? row.cells[5].style.color = "green" :  row.cells[5].style.color = "red";


                }
                }

            )

    }
    addRow();
    setInterval(updateRow, 1000);
    // setInterval(addRow, 10000);














    </script>













</html>
