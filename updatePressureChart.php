<?php

$node = $_GET["node_id"];

require_once './MeraniaController.php';
$meraniaController = new MeraniaController();

$merania =  $meraniaController->getMeraniaForUser($node);

$pole = [];
forEach($merania as $meranie){
    array_push($pole,  $meranie['pressure']);
}

echo json_encode($pole);


?>