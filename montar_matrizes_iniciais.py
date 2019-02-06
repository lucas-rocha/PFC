import os
import numpy as np
import json
import math
from scipy import stats
from scipy.spatial import distance

def calculo_ppmi(B, alpha, i, j):
    Pij = B[i][j]/alpha
    Pi = math.fsum(B[i])/alpha
    Pj = math.fsum(B[j])/alpha
    K = Pij/(Pi*Pj)
    if K == 0:
        return 0
    P = math.log10(K)
    if P > 0:
        return P
    return 0

def contagem_interseccao(a, b):
    s = set(a)
    return len(s.intersection(b))

def new_indice(lines):
    indice = {}
    contador = 0
    for l in lines:
        for w in l.split():
            if not w in indice.keys():
                indice[w] = contador
                contador += 1

def main():
    folder = "Documentos Finais/"
    output_X = "X/"
    matriz_coocorrencia = "Matrizes de Coocorrencia/"
    output_indice = "Indices de Termos/"
    matriz_PPMI = "Matrizes PPMI/"
    correlacao_pearson = "Correlacao de Pearson/"
    correlacao_cosseno = "Correlacao Cosseno/"

    for doc in os.listdir(folder):
        print("--> " + doc)

        file = folder + doc
        f = open(file, 'r')

        lines = f.readlines()

        indice_aux = {}  #mapeamento do termo para inteiro
        indice = {}  # mapeamento de inteiro para termo
        indice_ocorrencia = {}  #listas de documentos em que o termos aparecem
        contador = 0

        for index, l in enumerate(lines):
            for w in l.split():
                if not w in indice_aux.keys():
                    indice[contador] = w
                    indice_aux[w] = contador
                    contador += 1
                    indice_ocorrencia[indice_aux[w]] = [index]
                else:
                    indice_ocorrencia[indice_aux[w]].append(index)

        # Salvando indices Termo-Inteiro
        file = output_indice + (doc.split('.txt')[0]) + '.json'
        output = open(file, 'w+')
        out = json.dumps(indice)
        output.write(out)
        output.close()

        # Montando e salvando matriz X
        file = output_X + doc
        output = open(file, 'w+')

        X = np.zeros((len(indice_aux.keys()), len(lines)), dtype=np.int_)

        for termo in indice_ocorrencia.keys():
            for documento in indice_ocorrencia[termo]:
                X[termo][documento] += 1

        for i in range(0,len(indice_aux.keys())):
            for j in range(0,len(lines)):
                output.write(str(X[i][j]) + ' ')
            output.write('\n')

        indice_aux.clear()
        print('Matriz X (ok).')

        # Montando e salvando matriz de coocorrencia
        file = matriz_coocorrencia + doc
        output = open(file, 'w+')

        B = np.zeros((contador,contador), dtype=np.int_)
        alpha = 0

        for i in range(0,contador):
            for j in range(0,contador):
                B[i][j] = contagem_interseccao(indice_ocorrencia[i], indice_ocorrencia[j])
                output.write(str(B[i][j]) + ' ')
                alpha += B[i][j]
            output.write('\n')

        output.close()
        print('Matriz de Coocorrencia (ok).')

        # Montando e salvando matriz PPMI
        file = matriz_PPMI + doc
        output = open(file, 'w+')

        PPMI = np.zeros((contador, contador), dtype=np.float64)

        for i in range(0,contador):
            for j in range(0,contador):
                PPMI[i][j] = calculo_ppmi(B, alpha, i, j)
                output.write(str(PPMI[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz de PPMI (ok).')

        # Montando e salvando matriz de Correlação de Pearson
        file = correlacao_pearson + doc
        output = open(file, 'w+')

        pearson = np.zeros((contador, contador), dtype=np.float64)

        for i in range(0,contador):
            for j in range(0,contador):
                pearson[i][j], p_value = stats.pearsonr(PPMI[i],PPMI[j])
                output.write(str(pearson[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz de Correlação de Pearson (ok).')

        # Montando e salvando matriz de Correlação Cosseno
        file = correlacao_cosseno + doc
        output = open(file, 'w+')

        cosseno = np.zeros((contador, contador), dtype=np.float64)

        for i in range(0, contador):
            for j in range(0, contador):
                distancia = distance.cosine(PPMI[i], PPMI[j])
                cosseno[i][j] = 1 - distancia
                output.write(str(cosseno[i][j]) + ' ')
            output.write('\n')

        output.close()
        print('Matriz de Correlação Cosseno (ok).')

# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()
