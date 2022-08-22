<?php
/**
 * Created by PhpStorm.
 * User: mateusz
 * Date: 26.07.17
 * Time: 11:35
 */
class Person
{
    private $name;
    private $surname;
    private $bornYear;
    private $bornPlace;
    private $deathYear;

    public function getName()
    {
        return $this->name;
    }

    public function setName($name)
    {
        $this->name = $name;
    }

    public function getSurname()
    {
        return $this->surname;
    }

    public function setSurname($surname)
    {
        $this->surname = $surname;
    }

    public function getBornYear()
    {
        return $this->bornYear;
    }

    public function setBornYear($bornYear)
    {
        $this->bornYear = $bornYear;
    }

    public function getBornPlace()
    {
        return $this->bornPlace;
    }

    public function setBornPlace($bornPlace)
    {
        $this->bornPlace = $bornPlace;
    }

    public function getAge()
    {
        if ($this->isAlive()) {
            $presentYear = date('Y');
            $bornYear = $this->getBornYear();
            $age = $presentYear - $bornYear;
            return $age;
        } else {
            $deathYear = $this->getDeathYear();
            $bornYear = $this->getBornYear();
            $deathAge = $deathYear - $bornYear;
            return $deathAge;
        }
    }

    public function setAge($age)
    {
        $this->age = $age;
    }

    public function getDeathYear()
    {
        return $this->deathYear;
    }

    public function setDeathYear($deathYear)
    {
        $this->deathYear = $deathYear;
    }

    public function isAlive()
    {
        if ($this->deathYear == null) {
            return true;
        }
        return false;
    }
}