import os
import numpy as np
import json
import math
from scipy import stats

def calculo_ppmi(B, alpha, i, j):
    Pij = B[i][j]/alpha
    Pi = math.fsum(B[i])/alpha
    Pj = math.fsum(B[j]) / alpha
    K = Pij/(Pi*Pj)

    print(alpha)
    print(K)
    if K == 0:
        return 0
    P = math.log10(K)
    print(P)
    print('---------------------------------------------------')
    if P > 0:
        return P
    return 0

#def calculo_alpha(B):


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
    matriz_coocorrencia = "Matrizes de Coocorrencia/"
    output_indice = "Indices de Termos/"
    matriz_PPMI = "Matrizes PPMI/"
    correlação_pearson = "Correlação de Pearson/"
    for doc in os.listdir(folder):
        print("--> " + doc)

        file = folder + doc
        f = open(file, 'r')

        lines = f.readlines()

        indice = {}  #mapeamento do termo para inteiro
        indice_ocorrencia = {}  #tweets em que o termo aparece
        contador = 0

        for index, l in enumerate(lines):
            for w in l.split():
                if not w in indice.keys():
                    indice[w] = contador
                    contador += 1
                    indice_ocorrencia[indice[w]] = [index]
                else:
                    indice_ocorrencia[indice[w]].append(index)

        # Salvando indice Termo-Inteiro
        file = output_indice + (doc.split('.txt')[0]) + '.json'
        output = open(file, 'w+')
        out = json.dumps(indice)
        output.write(out)
        output.close()

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

        # Montando e salvando matriz de Correlação de Pearson
        file = correlação_pearson + doc
        output = open(file, 'w+')

        pearson = np.zeros((contador, contador), dtype=np.float64)

        for i in range(0,contador):
            for j in range(0,contador):
                pearson[i][j], p_value = stats.pearsonr(PPMI[i],PPMI[j])
                output.write(str(pearson[i][j]) + ' ')
            output.write('\n')

        output.close()


# -------------------------------------------------------------------------#
# Executa o metodo main
if __name__ == "__main__": main()