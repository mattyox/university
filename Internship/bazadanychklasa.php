<?php

require_once('Person.php');

$data = file_get_contents('dane2.txt');
$lines = explode("\n", $data);
for ($i = 0; $i <= 4; $i++) {
    $personData = explode(';', $lines[$i]);
    $person = new person();
    $person->setName($personData[0]);
    $name = $person->getName();
    $person->setSurname($personData[1]);
    $surname = $person->getSurname();
    $person->setBornYear($personData[2]);
    $bornYear = $person->getBornYear();
    $person->setBornPlace($personData[3]);
    $bornPlace = $person->getBornPlace();
    if (!isset($personData[4])) {
        $personData[4] = 0;
    }
    $person->setDeathYear($personData[4]);
    $age = $person->getAge();
    $deathYear = $person->getDeathYear();
    if ($person->isAlive()) {
        echo "$name $surname urodzony w $bornPlace w roku $bornYear obecnie ma $age lat." . PHP_EOL;
    } else {
        echo "$name $surname urodzony w $bornPlace w roku $bornYear zmarł w roku $deathYear w wieku $age lat." . PHP_EOL;
    }
}