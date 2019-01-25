import os
import numpy as np
from sklearn.decomposition import NMF, non_negative_factorization

def main():
    folder_X = "X/"
    folder_U = "U/"
    output_V = "V/"

    for doc in os.listdir(folder_X):
        print("--> " + doc)

        #---------- Carregando matriz U ----------#
        file = folder_U + doc
        f = open(file, 'r')

        lines = f.readlines()
        f.close()

        n_termos = len(lines)
        n_topicos = len(lines[0].split())

        U = []

        for i in range(0, n_termos):
            U.append([])
            colunas = lines[i].split()
            for j in range(0, n_topicos):
                U[i].append(float(colunas[j]))

        U = np.array(U)
        print('Matriz U pronta')

        #--------- Carregando matriz X ----------#
        file = folder_X + doc
        f = open(file, 'r')

        lines = f.readlines()
        f.close()

        n_documentos = len(lines[0].split())

        X = []

        for i in range(0, n_termos):
            X.append([])
            colunas = lines[i].split()
            for j in range(0, n_documentos):
                X[i].append(int(colunas[j]))

        X = np.array(X)

        print('Matriz X pronta')

        #---------- Gerando matriz aleatória V ----------#
        V = np.random.rand(n_topicos, n_documentos)

        #---------- Calculando Fatorização e gravando matriz V ----------#
        print('Calculando Fatorização...')

        W, H, n_iter = non_negative_factorization(X.T, n_components=n_topicos, init='custom', random_state=0, update_H=False,
                                                  H=U.T, W=V.T)

        file = output_V + doc
        output = open(file, 'w+')

        for i in range(0, n_topicos):
            for j in range(0, n_documentos):
                output.write(str(W.T[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz V pronta')

        #-----------------------------------------------------------#
        R = np.dot(W, H)
        R = R.T
        file = doc
        output = open(file, 'w+')

        for i in range(0, len(R)):
            for j in range(0, len(R[0])):
                output.write(str(R[i][j]) + ' ')
            output.write('\n')

        output.close()

# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()
