import os
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer


def matrix_factorization(R, P, Q, K, steps=5000, alpha=0.0002, beta=0.02):
    for step in range(steps):
        for i in range(len(R)):
            for j in range(len(R[i])):
                if R[i][j] > 0:
                    eij = R[i][j] - np.dot(P[i, :], Q[:, j])
                    for k in range(K):
                        #P[i][k] = P[i][k] + alpha * (2 * eij * Q[k][j] - beta * P[i][k])
                        Q[k][j] = Q[k][j] + alpha * (2 * eij * P[i][k] - beta * Q[k][j])

        e = 0
        for i in range(len(R)):
            for j in range(len(R[i])):
                if R[i][j] > 0:
                    e = e + pow(R[i][j] - np.dot(P[i, :], Q[:, j]), 2)
                    for k in range(K):
                        e = e + (beta / 2) * (pow(P[i][k], 2) + pow(Q[k][j], 2))
        if e < 0.001:
            break
    return Q


def main():
    folder_X = "Documentos Finais/"
    folder_U = "U/"
    output_V = "V/"

    for doc in os.listdir(folder_U):
        print("--> " + doc)

        # Carregando matriz U
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
        print('Matriz U pronta.')

        # Carregando matriz X
        file = folder_X + doc
        f = open(file, 'r')

        lines = f.readlines()
        f.close()

        vectorizer = CountVectorizer()
        X = vectorizer.fit_transform(lines)
        X = X.toarray().T

        n_documentos = len(X[0])
        print('Matriz X pronta.')

        # Gerando matriz aleatória V
        V = np.random.rand(n_topicos, n_documentos)

        # Calculando Fatorização e gravando matriz V
        print('Calculando Fatorização.')
        V = matrix_factorization(X, U, V, n_topicos)
        ##nX = np.dot(U, V)

        file = output_V + doc
        output = open(file, 'w+')

        for i in range(0, n_topicos):
            for j in range(0, n_documentos):
                output.write(str(V[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz V pronta.')

# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()
