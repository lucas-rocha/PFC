from stopwords import stop_words

def stopwords(line, ego):
    words = line.split()
    wordsFiltered = []

    for w in words:
        if (not w == ego) and (w not in stop_words):
            wordsFiltered.append(w)

    return wordsFiltered


def contagem(lines, t):
    cont = 0
    for line in lines:
        l = line.split()
        for termo in l:
            if termo == t:
                cont+=1
                break

    return cont

def main():
    import os
    folder = "egos/"
    output = "Documentos Finais/"

    for doc in os.listdir(folder):
        print("\n--> " + doc)

        file = folder + doc
        f = open(file, 'r', encoding="utf8")

        doc_save = output + doc
        outfile = open(doc_save, 'a+')

        ego = doc.split('.txt')[0]

        lines = f.readlines()

        termos = {}

        for line in lines:
            tweet = stopwords(line, ego)
            if len(tweet) > 0:
                cont = 0
                for termo in tweet:
                    if not termo in termos.keys():
                        termos[termo] = contagem(lines, termo)
                    #Salvando
                    if termos[termo] > 1:
                        outfile.write(str(termo + " "))
                        cont += 1
                if cont > 0:
                    outfile.write(str("\n"))

        f.close()
        outfile.close()
# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()
