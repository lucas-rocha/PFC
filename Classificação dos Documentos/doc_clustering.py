import os
import numpy as np

for doc in os.listdir():
    if doc.split('.')[1] != 'txt':
        continue
    print('--> ' + doc)

    #--------- Carregando matriz de classificação ----------#
    f = open(doc, 'r')

    lines = f.readlines()
    f.close()

    n_documentos = len(lines)
    n_topicos = len(lines[0].split())

    X = []

    for i in range(0, n_documentos):
        X.append([])
        colunas = lines[i].split()
        for j in range(0, n_topicos):
            X[i].append(float(colunas[j]))

    X = np.array(X)

    #--------- Carregando documentos ----------#
    doc_folder = '../Documentos Finais/'
    file = doc_folder + doc
    f = open(file, 'r', encoding="utf8")
    documentos = f.readlines()
    f.close()

    #--------- Carregando classificação ----------#
    classificacao = []

    for d in X:
        classificacao.append(np.argmax(d))

    #--------- Montando e salvando clustering dos documentos ----------#
    ego = doc.split('.')[0]
    file = ego + '.group'
    output = open(file, 'w+')

    generic_topic = []

    for t in range(0, n_topicos):
        output.write('# Cluster ' + str(t) +':\n')
        for i, j in enumerate(classificacao):
            if j == t:
                if j == 0 and np.amax(X[i]) == 0:
                    generic_topic.append(documentos[i])
                else:
                    output.write(documentos[i])
        output.write('\n')

    output.write('# Cluster Generico:\n')
    for d in generic_topic:
        output.write(d)

    output.close()