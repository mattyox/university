<?php

require_once('Person.php');

class PersonRepository
{
    public function getAll($dataFileInput)
    {
        $data = file_get_contents("$dataFileInput");
        $line = explode("\n", $data);
        $people = [];
        for ($i = 0; $i <= 4; $i++) {
            $personData = explode(';', $line[$i]);
            $person = new person();
            $person->setName($personData[0]);
            $person->setSurname($personData[1]);
            $person->setBornYear($personData[2]);
            $person->setBornPlace($personData[3]);
            if (isset($personData[4])) {
                $person->setDeathYear($personData[4]);
            }
            $people[$i] = $person;
        }
        return $people;
    }

    /**
     * @param $person person
     * @return null
     */
    public function save($dataFileOutput, $person)
    {
        $name = $person->getName();
        $surname = $person->getSurname();
        $bornYear = $person->getBornYear();
        $bornPlace = $person->getBornPlace();
        $deathYear = $person->getDeathYear();
        $line = "$name;$surname;$bornYear;$bornPlace;$deathYear";
        file_put_contents("$dataFileOutput", $line);
        return null;
    }
}

$dataFileInput = 'dane2.txt';
$dataFileOutput = 'dane3.txt';
$personRepository = new PersonRepository();
$people = $personRepository->getAll($dataFileInput);
$personRepository->save($dataFileOutput, $people);