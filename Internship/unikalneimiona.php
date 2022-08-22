<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 13.07.17
 * Time: 12:34
 */
$names = [];
//$input = $argv;
$input = array_slice($argv, 1);
foreach ($input as $name) {
    $name = ucfirst($name);
}
$uniqueNames = array_unique($input);
$uniqueNamesPrint = implode(", ", $uniqueNames);
echo $uniqueNamesPrint . PHP_EOL;