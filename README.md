# PORR
zbiór danych:
zbiór danych: https://www.kaggle.com/bittlingmayer/amazonreviews

Podsumowanie badań

Stacja:
Lenovo Ideapad, Intel(R) Core(TM) i5-6300HQ CPU @ 2.30GHz (4 wątki), GeForce GTX 950M, Ram: 8GB DDR3
Liczba wątków omp: 4

| Liczba wątków omp  | Czas wykonania bez optymalizacji | Czas wykonania z optymalizacją | Procentowe skrócenie czasu |
| ------------------ | -------------------------------- | ------------------------------ | -------------------------- |
|1|22926|22924|0.008 % |
|2|22926|12830|44.037 % |
|3|22926|8948|60.970 % |
|4|22926|7274|68.272 % |

Stacja:
Komputer stacjonarny, AMD Ryzen 9 5950X 16-Core (32 wątki), Quadro RTX 4000, Ram: 32GB DDR4
| Liczba wątków omp  | Czas wykonania bez optymalizacji | Czas wykonania z optymalizacją | Procentowe skrócenie czasu |
| ------------------ | -------------------------------- | ------------------------------ | -------------------------- |
|2|10911|7092|35.0014
|4|10911|3805|65.1269
|6|11146|2607|76.6104
|8|10900|1972|81.9083
|10|11516|1714|85.1164
