import findspark
import time
from collections import defaultdict
from typing import List, Tuple

findspark.init()
DATA_FILE = "data.txt"

from pyspark import SparkContext, SparkConf

def print_sorted_count_list(count_list: List[Tuple[str, int]], word_num: int):
    # sortowanie danych i wypisanie najczestszych slow
    count_list.sort(reverse=True, key = lambda x: x[1])
    
    for (word, count) in count_list:
        if word_num <= 0:
            break

        print(f'{word}: {count}')
        word_num -= 1

def compare_lists(l1, l2):
    l1.sort()
    l2.sort()
    if l1 == l2:
        print('Equal')
    else:
        print('Not equal!')

    for i, value in enumerate(l1):
        if l1[i] != l2[i]:
            print(f'{l1[i]} is not equal to {l2[i]}')
            return

if __name__ == "__main__":

    print(f'Opening file and reading words')
    fd = open(DATA_FILE, 'r')
    read_data = fd.read()
    words = read_data.split()

    word_counts_single_thread = defaultdict(lambda: 0)
    
    print(f'Counting words on single thread is started')
    start = time.time()
    for word in words:
        word_counts_single_thread[word] += 1

    end = time.time()
    diff_single_thread = end - start
    print(f'Word calculation without spark took: {diff_single_thread}')
    # przygotowanie listy krotek w celu wyswietlenia wartosci oraz porownania
    # z wynikami po przetworzeniu przez Spark'a

    # reading words by read() removes '' character
    # it is the only difference between word count list and Spark's word count list
    # it is manually added to make comparison of these lists easier
    word_counts_single_thread[''] += 1

    total_single_thread = []
    for key, value in word_counts_single_thread.items():
        total_single_thread.append((key, value))

    total_single_thread.sort(reverse=True, key = lambda x: x[1])
    print(f'Single thread list size is: {len(total_single_thread)}')
    print_sorted_count_list(total_single_thread, 50) 

    sc = SparkContext("local", "PySpark Word Count Exmaple")

    # odczyt danych z pliku i podzial ich na linie
    words = sc.textFile(DATA_FILE).flatMap(lambda line: line.split(" "))

    start = time.time()
    # oblicza wystapienia kazdego slowa
    word_counts = words.map(lambda word: (word, 1)).reduceByKey(lambda a, b: a + b)
    
    # agregacja danych
    total = word_counts.collect()

    end = time.time()
    diff = end - start
    print(f'Word calculation with spark took: {diff}')
    print(f'Spark list size is: {len(total)}')
    print_sorted_count_list(total, 50) 

    # zapis wynikow z posczegolnych klastrow
    word_counts.saveAsTextFile("output")
    compare_lists(total_single_thread, total)
    print(f'Spark time: {diff}, single thread time: {diff_single_thread}')

