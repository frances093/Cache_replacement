# Cache_replacement
## 簡介：  
* 實作 Cache replacement。
   * FIFO：first in first out,先進先出。
   * LRU：least recently used，最近最少使用，判斷最近被使用的時間。

* 使用語言：C++。
## Input：
* 檔案：（要和程式放在同一層）
    一個 .txt 檔，裡面包含數個（一行一個）以十六進為表示的 address 。

        C64E5
        C64C5
        C74EA
        C64E8
        C64CF
        F64E6
        F74EA
        F84E6
        C74CF
        C64E6
        C64C9
        C74E8

* 執行視窗：輸入 1 或 0，用於決定要使用 FIFO（0） 還是 LRU（1） 作為 replacement 的方案。

## Output：
* 執行視窗：

        # of Offset bits: 4
        # of Index bits: 8
        # of Tag bits: 8

        Input 0(for FIFO) or 1(LRU) to determine using which mode: 0

        Using FIFO for cache replacement!

        Address Hit?    Putin   Replace?
        C64E5   Miss!   312
        C64C5   Miss!   304
        C74EA   Miss!   313
        C64E8   Hit!
        C64CF   Hit!
        F64E6   Miss!   314
        F74EA   Miss!   315
        F84E6   Miss!   312     Yes!
        C74CF   Miss!   305
        C64E6   Miss!   313     Yes!
        C64C9   Hit!
        C74E8   Miss!   314     Yes!

        Total miss time: 9
        Total replace time: 3

## Step：
1. 讀檔，讀入要放入 Cache 的 data 的 address ：

        void readfile() 

2. 按照設定的數量產生相應數量的 block ：

        void generateCache(int n)

3. 根據使用者輸入的內容決定要使用何種 method 作為 replacement 方法。
4. 依序選取各個 address 。
5. 將選取的十六進位 address 轉成二進位：

        string HextoBinary(address[i])

    * 此 function 會再呼叫：

            string getbinary(char a)

        協助將 address 中的各個字元逐一轉成四位二進位表示。

6. 按照預設，自二進位 address 中分割出 index 和 tag ：

        int getTagorIndex(int num, int skip, string a)

7. 按照 6. 得到的結果，access 2. 建立的 block 。如果有 index 和 tag 皆符合的則 print 出 "Hit!" ，反之 print 出 "miss!"：

        bool hit(Block *ptrb, int tag, int cycle)
    * 在此步中，若一開始選擇 LRU 遇到皆相符的會更新該 block ， FIFO 則否。
        
8. 若 7. 中未找到相符的，則逐個掃描相同 index 的 block 是否有空的，若有找到則放入目前的 address ，再 print 出放入哪個 block：

        bool Putin(Block *ptrb, int tag, int cycle)

9. 若 8. 未找到空的 block 則尋找上次取用時間距離現在最久的。此步會呼叫：

        int findrec(Block *ptrb)

    協助比對各個同 index 最近取用的時間。

10. 依 9. 的結果將 block 內存放的 address 更改成目前的 address ：

        void replace(Block *ptrb, int tag, int nowcycle)

    在 print 出 "Yes!" 。
11. 直到掃完所有 address 為止，重複 4.~10. 。
11. print 出總計 miss 及 replace 多少次。
12. 結束運行。
