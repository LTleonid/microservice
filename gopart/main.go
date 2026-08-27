package main //Надо всегда начинать с пакагов
import (
	"fmt"
	"math"
)

func main() {
	var height = 1.8
	var weight float64 = 100
	var imt = weight / math.Pow(height, 2)
	fmt.Print(imt)
}

// var imt = float64(weight) / weight // Эт явные преобразы
