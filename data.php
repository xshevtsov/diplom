<?php

include 'config.php';
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PATCH, PUT, DELETE, OPTIONS');
header('Access-Control-Allow-Headers: Origin, Content-Type, X-Auth-Token');
header("Access-Control-Allow-Credentials: true ");

//$api_key = 'api-key';
//if($_SERVER != $api_key){
////    echo json_encode(['api-key' => 'error']);
//    echo $_SERVER;
//    return;
//}


require_once './sensorsController.php';
$sensorsController = new SensorsController();

require_once './MeraniaController.php';
$meraniaController = new MeraniaController();


$post_data = ($data = json_decode(file_get_contents('php://input'), true));


if($_SERVER['REQUEST_METHOD'] === "POST"){




//    $json = file_get_contents('./storage/mesh.json');
//    $json_data = json_decode($json,true);

//    $createMeranie = $meraniaController->createMeranie($json_data[0]['node'],$json_data[0]['temp'], $json_data[0]['alt'],$json_data[0]['pres']);


    foreach($post_data as $item) { //foreach element in $arr
        $meraniaController->createMeranie($item['node'],$item['temp'], $item['alt'],$item['pres']);

    }




    $fp = fopen('./storage/mesh.json', 'w');
    fwrite($fp, json_encode($post_data));
    fclose($fp);
    return;

}




//if($post_data['api_key'] != ''){
//    echo json_encode(['api-key' => 'error']);
//    return;
//}



//if($post_data['api_key'] != $api_key){
//    echo json_encode(['api-key' => 'error']);
//    return;
//}


//echo $api_key['test_string'];

// if($api_key['api_key'] != 'API-KEY-SECRET'){
//     echo json_encode(['api-key' => 'error']);
//     return;
// }

$json = file_get_contents('./storage/mesh.json');

// Decode the JSON file
$json_data = json_decode($json,true);



echo json_encode($json_data);

