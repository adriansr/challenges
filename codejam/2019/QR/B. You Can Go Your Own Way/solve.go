package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

type InputReader struct {
	*bufio.Reader
}

func (r InputReader) ReadLine() string {
	line, _ := r.Reader.ReadString('\n')
	n := len(line)
	if n > 0 {
		return line[:n-1]
	}
	return line
}

func (r InputReader) ReadInt() int {
	n, _ := strconv.Atoi(r.ReadLine())
	return n
}

func NewInputReader() InputReader {
	return InputReader {
		Reader: bufio.NewReader(os.Stdin),
	}
}

func main() {
	reader := NewInputReader()
	T := reader.ReadInt()
	for testNum := 1; testNum <= T; testNum ++ {
		N := reader.ReadInt()
		path := reader.ReadLine()
		if len(path) != 2*N - 2 {
			panic(fmt.Sprintf("Wrong length %d vs N=%d", len(path), N))
		}
		result := solve(N, []byte(path))
		fmt.Printf("Case #%d: %s\n", testNum, string(result))
	}
}

func solve(N int, advPath []byte) []byte {
	m := buildUsedMap(advPath)
	//fmt.Println(m)
	lim := 2 * N - 2
	result := make([]byte, lim)
	index := 0
	var pos cell
	for index < lim {
		adv, _ := m[pos]
		if adv != 'B' {
			self := result[index]
			if self == 0 && adv != 'E' && pos.x < N-1 {
				// Go East at pos
				result[index] = 'E'
				pos.x += 1
				index++
				continue
			}
			if self != 'S' && adv != 'S' && pos.y < N-1 {
				// Go South at pos
				result[index] = 'S'
				pos.y += 1
				index++
				continue
			}
			// Can't go East nor South, backtrack
			//fmt.Printf("Backtrack pos=%v index=%4d '%s' '%s'\n", pos, index, string(advPath), string(result[:index]))
			result[index] = 0
			m[pos] = 'B' // Block this cell forever
		}
		index --
		if index == -1 {
			panic("Unexpected index == -1")
		}
		switch result[index] {
		case 'E':
			pos.x --
		case 'S':
			pos.y --
		}
	}
	return result
}

type cell struct {
	x, y int
}

func buildUsedMap(advPath []byte) map[cell]byte {
	var pos cell
	m := make(map[cell]byte, len(advPath)+2)
	for _, dir := range advPath {
		m[pos] = dir
		switch dir {
		case 'E':
			pos.x += 1
		case 'S':
			pos.y += 1
		}
	}
	if pos.x != pos.y {
		panic("Uneven adversary path")
	}
	return m
}
