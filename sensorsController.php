<?php

require_once "./DB_PDO.php";

class SensorsController
{
    protected DB_PDO $pdo;

    public function __construct()
    {
        $this->pdo = new DB_PDO();
    }

    public function createNode(string $id)
    {
        try {
            $this->pdo->run("INSERT INTO `nodes` (`id`) VALUE (?)", [$id]);
            return $this->findNodeById($id);
        } catch (Exception $e) {
            return null;
        }
    }

    public function findNodeById($id)
    {
        try {
            $node = $this->pdo->run("SELECT * FROM `nodes` WHERE id=?", [$id])->fetch();
            return $node;
        } catch (Exception $e) {
            return null;
        }
    }


    public function deteleNode($id): bool
    {
        try {
            $this->pdo->run("DELETE FROM `nodes` WHERE `id`=?", [$id]);
            return true;
        } catch (Exception $e) {
            return false;
        }
    }
}
