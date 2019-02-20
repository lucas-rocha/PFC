import os

for doc in os.listdir():
    if doc.split('.')[1] != 'net':
        continue
    print('--> ' + doc)
    
    f = open(doc, 'r')
    lines = f.readlines()
    f.close()

    new_lines = []

    new_lines.append(lines[0])
    
    n_vertices = int(lines[0].split()[1])
    
    aux = 1
    vert = []

    for l in lines[1:n_vertices + 1]:
        if int(l.split()[0]) == aux:
            new_lines.append(l)
            aux+=1
        else:
            vert.append(l)

    for v in vert:
        nv = str(aux) + ' ' + v.split()[1] +'\n'
        new_lines.append(nv)
        aux+=1

    outfile = open(doc, 'w+')

    for l in new_lines:
        outfile.write(l)

    for l in lines[n_vertices+1:]:
        outfile.write(l)

    outfile.close()