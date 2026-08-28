package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
)

const SharpURL = "http://sharp:8080/api/Process"

// const SharpURL = "https://httpbin.org/post"

type DataText struct {
	Text    *string  `json:"text"`
	Provide []string `json:"provide"` //фига забавно массивы
}

func gateway(w http.ResponseWriter, r *http.Request) {
	if r.Method == http.MethodPost {
		ctx_byte, err := io.ReadAll(r.Body)
		if err != nil {
			fmt.Printf("Error %v", err)
			return
		}
		ctx := string(ctx_byte)
		fmt.Print(ctx)
		var data DataText
		data.Text = &ctx
		data.Provide = append(data.Provide, "go") //лять, серъёзно?? реасигн вместо n+1, чзх
		r.Body.Close()
		jsonpayload, err := json.Marshal(data)
		if err != nil {
			fmt.Println(err)
			return
		}
		req, err := http.NewRequest(
			http.MethodPost,
			SharpURL,
			bytes.NewBuffer(jsonpayload),
		)
		if err != nil {
			fmt.Println(err)
			return
		}
		req.Header.Set("Content-Type", "application/json")
		res, err := http.DefaultClient.Do(req)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Println(res.StatusCode)
		sres, err := io.ReadAll(res.Body)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Fprintln(w, string(sres))
		res.Body.Close()

	}
}

func main() {
	print("handle /")
	http.HandleFunc("/", gateway)
	print("start serve")
	http.ListenAndServe(":8080", nil)
}
