<?php

require_once "config.php";

class DB_PDO extends PDO
{

    public function __construct($options = [])
    {
        $default_options = [
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
            PDO::ATTR_EMULATE_PREPARES => false,
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        ];
        $options = array_replace($default_options, $options);
        $dsn = "mysql:host=" . DB_HOST . ";dbname=" . DB_NAME;
        parent::__construct($dsn, DB_USER, DB_PASSWORD, $options);
    }
    public function run($sql, $args = NULL)
    {
        if (!$args)
        {
            return $this->query($sql);
        }
        $stmt = $this->prepare($sql);
        $stmt->execute($args);

        return $stmt;
    }
}