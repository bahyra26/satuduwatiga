kalo saya coba pahamin jadi awalnya ini itu intinya sebuah robot yang mau main sepak bola

trus dia harus scanning dengan visionnya yang jalan jalan, bentuk vision itu segitiga dan dia bisa muter 90 derajat

trus abisitu si robot ketika nemuin bola, dia harus jalan ke bola itu

nah ketika udah didepan pas bola itu baru di shoot ke gawang, bisa lurus ataupun bisa miring gitu si bolanya

#langkah ku ngerjain (karna wajib OOP)

1. bikin lapangannya dulu, karena spesifikasinya cukup lengkap yaitu kan uk lapangan itu 9 m x 6 m
tapi tiap 1 petak itu merepresentasikan 0.5 meter, jadinya 9/(0.5) x 6/(0.5) = 18 m x 12 m

2. setelah itu aku nambahin gawang di bagian kanan lapangan dengan # sebanyak 6 petak atau sebesar 6x0.5 = 3 meter aja

3. plotting bola, aku pengen bikin sebuah kelas buat naruh file .h nya dari si bola, agar enak naruh bolanya dan nanti tinggal dipanggil di function main

4. membuat function logic biar nanti si bolanya itu ngetes apakah si bola bisa berjalan lurus apa ga ataupun secara diagonal, sesuai dengan logic yang diinginkan

5. membuat robot, membuat function robot biar si robot ini nanti bisa di plotting darimana aja, dan robotnya itu nanti menghadap ke arah mana juga dan membuat si robot bisa tau bahwa gawangnya ada di mana

6. membuat si robot, berjalan untuk mulai scanning bola bolanya

7. setelah scanning udah bisa dilakukan, barulah membuat function gimana caranya si robot bisa menuju ke arah belakang bola dan nanti mulai menendang, nah menendang ini kembali ke nomor 4

