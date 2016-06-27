// A Line Comment for
// the package.
package main

/*
	A multiline comment
	* A List
	* B List
	* /*
 */

import (
	"flag"
)

import "fmt"

const ENV_NAME = "TEST"

const (
        Monday = iota
        Tuesday
        Wednesday
        Thursday
        Friday
        Saturday
        Sunday
)

var (
        config = flag.String("config", "", "Specify a config file")
)

type MyUInt8 uint8
type MyUInt16 uint16
type MyUInt32 uint32
type MyUInt64 uint64

type MyInt8 int8
type MyInt16 int16
type MyInt32 int32
type MyInt64 int64

type MyFloat32 float32
type MyFloat64 float64

type MyComplex64 complex64
type MyComplex128 complex128

type MyByte byte
type MyRune rune

type MyInt int
type MyUInt uint

type MyUIntPtr uintptr

type MyBool bool

type MyStruct struct {
	IntsField []int `protobuf:"2"`
	_ int // dummy
}

func (m *MyStruct) GetFirst() int {
	return m.IntsField[0]
}

type MyEmbedded struct {
	*MyStruct
}

type MyInterface interface {
	CallMe()
}

func mySum(values ...int) {}

func main() {
	flag.Parse() // Parse command line args

	// Test if else and strings
	if (*config != "") {
		fmt.Printf("Using config: %s\n", *config)
	} else if (true) {
		fmt.Println("No config specified")
	}

	// Test chan
	ch := make(chan int)
	ch <- 5

	// Test for select
	for {
		select {
		case <-ch: break;
	}

	// Test C-style loop
	for (i := 0; i < 2; i++) {
		fmt.Println(i)
	}

	// Test runes
	r1 := '本';
	r2 := '\'';

	// Test numbers
	n1 := 0
	n2 := 12
	n3 := 0600 // Octal
	n4 := 0xFfA
	n5 := 1.e+0
	n6 := .25
	n7 := .12E+5
	n8 := 011i
	n9 := 2.75E+5i
        n10 := 0900 // Invalid Octal

	// Test multiline string
	ms := `
	multiline strings
	are awesome!!

	:) 本
`

	// Test arrays and builtins
	ar1 := []int{1, 2}

	len(a)
	cap(a)

	ar2 := new([2]int)
	ar3 := make([]int, 0, 4)

	// Test map
	ma := map[string]int{}

        // Test operators
        + - * / %
        += -= *= /= %=
        < > << >> <<= <= >= >>= ^
        <-
        := : . , ;
        & && &^ &^= &=
        | || |=
        ++ --
        ! !=
        = ==
        () {} []

        // Builtin Functions
        cap()
        len()
        make()
        new()
        close()
        append()
        delete()
        copy()
        complex()
        real()
        imag()
        panic()
        recover()
}
