"""
Python Script to format text documents such that they can be loaded into
Document data structure and parsed by the Autocorrect Algorithm

Required format:
- single word per line
- no punctuation/special characters
- lowercase letters
"""
import shutil
import os
import sys
from os import listdir
from os import path
import string
from string import join
from tempfile import mkstemp

table = string.maketrans("","")


file_path = os.path.join(os.getcwd(),"docs")

files = [f for f in listdir(file_path) if f.find("lower_")==-1]
newline = ["\n\n", "\n\r", "\r\n", "\n", "\r"]
exclude = set(string.punctuation)

print(files)

for file_name in files:
    print(file_name)
    infile_path = file_path + "/" + file_name
    outfile_path = file_path + "/lower_" + file_name

    #fh, abs_path = mkstemp()
    #with open(abs_path, 'w') as outfile:
    with open(outfile_path, 'w+') as outfile:
        with open(infile_path) as infile:
            for line in infile:
                text = line.split(" ")
                for word in text:
                    if not word or word in newline:
                        continue
                    if word.find("\n")!=-1 or word.find("\r")!=-1:
                        for symbol in newline:
                            word = word.replace(symbol, "")
                    word = "".join(ch for ch in word if ch not in exclude)
                    if word:
                        word = word.lower() + "\n"
                        outfile.write(word);
    infile.close()
    outfile.close()
