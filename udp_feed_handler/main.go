package main

import (
    "encoding/binary"
    "fmt"
    "log"
    "net"
)

type Quote struct {
    Ts   uint64
    Bid  uint32
    Ask  uint32
    Size uint32
}

func parseQuote(pkt []byte) Quote {
    return Quote{
        Ts:  binary.LittleEndian.Uint64(pkt[0:8]),
        Bid: binary.LittleEndian.Uint32(pkt[8:12]),
        Ask: binary.LittleEndian.Uint32(pkt[12:16]),
        Size: binary.LittleEndian.Uint32(pkt[16:20]),
    }
}

func main() {
    conn, err := net.ListenPacket("udp", ":9000")
    if err != nil {
        log.Fatal(err)
    }
    buf := make([]byte, 1500)
    for {
        n, _, err := conn.ReadFrom(buf)
        if err != nil {
            log.Fatal(err)
        }
        q := parseQuote(buf[:n])
        fmt.Printf("%d %d %d %d\n", q.Ts, q.Bid, q.Ask, q.Size)
    }
}
