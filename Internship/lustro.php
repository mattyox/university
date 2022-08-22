<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 12.07.17
 * Time: 13:18
 */
$name = "Giovanni Paolo Secondo";
$reserveName = '';

$string = $name;
$lenght = strlen($name);
for ($i = $lenght; $i >= 0; $i--) {
    $reserveName = $reserveName . $string[$i];
}

echo $reserveName == 'odnoceS oloaP innavoiG' ? "Bardzo dobrze" : "Nie tak dobrze";
echo PHP_EOL;