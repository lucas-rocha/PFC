import os
import numpy as np
from scipy.spatial import distance

def main():
    folder_X = "X/"
    correlacao_cosseno = "Cosseno da Matriz Xt/"

    for doc in os.listdir(folder_X):
        print("--> " + doc)

        #--------- Carregando matriz X ----------#
        file = folder_X + doc
        f = open(file, 'r')

        lines = f.readlines()
        f.close()

        n_documentos = len(lines[0].split())
        n_termos = len(lines)

        X = []

        for i in range(0, n_termos):
            X.append([])
            colunas = lines[i].split()
            for j in range(0, n_documentos):
                X[i].append(int(colunas[j]))

        X = np.array(X)
        X = X.T

        print('Matriz X.T pronta')

        #---------- Montando e salvando matriz de Correlação Cosseno ----------#
        file = correlacao_cosseno + doc
        output = open(file, 'w+')

        cosseno = np.zeros((n_documentos, n_documentos), dtype=np.float64)

        for i in range(0, n_documentos):
            for j in range(0, n_documentos):
                distancia = distance.cosine(X[i], X[j])
                cosseno[i][j] = 1 - distancia
                output.write(str(cosseno[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz de Correlação Cosseno (ok).')



# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()
