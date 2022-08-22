<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 25.07.17
 * Time: 10:48
 */
require_once('Person.php');

class AverageAgeCalculator
{

    private $people = [];

    public function addPerson($person)
    {
        $this->people[] = $person;
    }

    public function getAverageAge()
    {
        $ageAmount = 0;
        foreach ($this->people as $person) {
            $ageAmount = $ageAmount + $person->getAge();
        }
        $personNumber = count($this->people);
        $averageAge = $ageAmount / $personNumber;
        return $averageAge;
    }

    public function getAverageLifespan()
    {
        $ageAmount = 0;
        $personNumber = 0;
        foreach ($this->people as $person) {
            if ($person->isAlive()) {
                continue;
            }
            $ageAmount = $ageAmount + $person->getAge();
            $personNumber++;
        }
        $averageLifespan = $ageAmount / $personNumber;
        return $averageLifespan;
    }
}

$data = file_get_contents('dane2.txt');
$lines = explode("\n", $data);
$averageAgeCalculator = new AverageAgeCalculator();
for ($i = 0; $i <= 4; $i++) {
    $personData = explode(';', $lines[$i]);
    $person = new person();
    $person->setBornYear($personData[2]);
    $bornYear = $person->getBornYear();
    if (!isset($personData[4])) {
        $personData[4] = 0;
    }
    $person->setDeathYear($personData[4]);
    $age = $person->getAge();
    $deathYear = $person->getDeathYear();
    $averageAgeCalculator->addPerson($person);
}
$averegeAge = $averageAgeCalculator->getAverageAge();
echo "Średni wiek to $averegeAge lat" . PHP_EOL;
$averageLifespan = $averageAgeCalculator->getAverageLifespan();
echo "Średnia długość życia to $averageLifespan lat." . PHP_EOL;
