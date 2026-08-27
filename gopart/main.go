package main //Надо всегда начинать с пакагов
import (
	"fmt"
	"math"
)

func main() {
	// height := 1.8
	// var weight float64
	// weight = 100
	// height, weight := 1.8 , 100 // Динам.-стат. распоковка
	const imtpower float64 = 2
	var height, weight float64 = 1.8, 100 // множественная распоковка
	var imt = weight / math.Pow(height, imtpower)
	fmt.Print(imt)
}

// var imt = float64(weight) / weight // Эт явные преобразы
