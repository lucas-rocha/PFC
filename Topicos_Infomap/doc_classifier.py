import os

output_folder = '../Classificação dos Documentos/'
doc_folder = '../Documentos Finais/'

for doc in os.listdir():
    if doc.split('.')[1] != 'ftree':
        continue
    print('--> ' + doc)

    #--------- Carregando Topicos/Clusters ----------#
    f = open(doc, 'r')
    lines = f.readlines()
    f.close()

    n_cluster = 0
    cluster = []

    for l in lines[2:]:
        if l[0] == '*':
            break

        spl = l.split()
        if int(spl[0][0]) == n_cluster:
            cluster[n_cluster-1].append(spl[2][1:-1])
        else:
            n_cluster += 1
            cluster.append([spl[2][1:-1]])

    #--------- Carregando documentos ----------#
    ego = doc.split('.')[0]
    file = doc_folder + ego + '.txt'
    f = open(file, 'r', encoding="utf8")
    documentos = f.readlines()
    f.close()

    #---------- Montando e salvando matriz de classificação ----------#
    file = output_folder + ego + '.txt'
    output = open(file, 'w+')

    for doc in documentos:
        termos = doc.split()
        for topic in range(0, n_cluster):
            count = 0
            for t in termos:
                if t in cluster[topic]:
                    count+=1
            k = count/len(cluster[topic])
            output.write(str(k) + ' ')
        output.write('\n')
    
    output.close()