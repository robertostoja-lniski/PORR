import findspark
findspark.init()

import time

from pyspark import SparkContext, SparkConf

if __name__ == "__main__":
    sc = SparkContext("local", "PySpark Word Count Exmaple")

    # odczyt danych z pliku i podzial ich na linie
    words = sc.textFile("data.txt").flatMap(lambda line: line.split(" "))

    start = time.time()
    # oblicza wystapienia kazdego slowa
    word_counts = words.map(lambda word: (word, 1)).reduceByKey(lambda a, b: a + b)
    
    # agregacja danych
    total = word_counts.collect()

    end = time.time()
    diff = end - start
    print(f'Word calculation with spark took: {diff}')
    
    # sortowanie danych i wypisanie najczestszych slow
    total.sort(reverse=True, key = lambda x: x[1])
    
    word_num = 50
    for (word, count) in total:
        if word_num <= 0:
            break

        print(f'{word}: {count}')
        word_num -= 1

    # zapis wynikow z posczegolnych klastrow
    word_counts.saveAsTextFile("output")

