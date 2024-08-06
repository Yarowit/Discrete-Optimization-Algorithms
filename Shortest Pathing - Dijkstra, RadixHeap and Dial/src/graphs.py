import numpy as np
import matplotlib.pyplot as plt
import os

def beg():

    points = 100

    f = open("results.txt","r")
    f.readline()
    Bn = []
    Un = []
    Ln = []
    Cn = []
    Dn = []
    DCn = []

    for n in range(points):
        dataArr = [[],[],[],[],[],[]]
        
        for i in range (50):
            for j in range(6):
                dataArr[j].append( (int) (f.readline()) )
        Bn.append(dataArr[0])
        Un.append(dataArr[1])
        Ln.append(dataArr[2])
        Cn.append(dataArr[3])
        Dn.append(dataArr[4])
        DCn.append(dataArr[5])

    data = [Bn,Un,Ln,Cn,Dn,DCn]
    averages = [[],[],[],[],[],[]]
    labels = ["Bn","Un","Ln","Cn","Dn","Dn - Cn"]
    for i in range(6):
        plt.figure()

        for j in range(points):
            plt.scatter([(j+1)*1000] * 50, data[i][j], color = 'blue')
            averages[i].append(np.average(data[i][j]))

        plt.scatter(range(1000, (points+1)*1000, 1000), averages[i], color = 'red')
        plt.ylabel(labels[i])
        plt.xlabel("Urny")
        plt.gcf().set_size_inches(16, 9)
        plt.savefig(str(i)+"fig.svg")

def plots():
    xax = range(1000, (points+1)*1000, 1000)
    labels = [
        "b(n) / n",
        "b(n) / √n",
        "u(n) / n",
        "l(n) / ln(n)",
        "l(n) / ( ln(n) / ln(ln(n)) )",
        "l(n) / ln(ln(n))",
        "c(n) / n",
        "c(n) / ( n ln(n) )",
        "c(n) / n^2",
        "d(n) / n",
        "d(n) / ( n ln(n) )",
        "d(n) / n^2",
        "d(n) - c(n) / n",
        "d(n) - c(n) / ( n ln(n) )",
        "d(n) - c(n) / ( n ln(ln(n)) )"
    ]

    functions = [
        [ averages[0][n]/(n) for n in range(points) ],
        [ averages[0][n]/(n**0.5) for n in range(points) ],

        [ averages[1][n]/(n) for n in range(points) ],

        [ averages[2][n]/(np.log(n)) for n in range(points) ],
        [ averages[2][n]/(np.log(n)/np.log(np.log(n))) for n in range(points) ],
        [ averages[2][n]/(np.log(np.log(n))) for n in range(points) ],

        [ averages[3][n]/(n) for n in range(points) ],
        [ averages[3][n]/(n*np.log(n)) for n in range(points) ],
        [ averages[3][n]/(n*n) for n in range(points) ],

        [ averages[4][n]/(n) for n in range(points) ],
        [ averages[4][n]/(n*np.log(n)) for n in range(points) ],
        [ averages[4][n]/(n*n) for n in range(points) ],
        
        [ averages[5][n]/(n) for n in range(points) ],
        [ averages[5][n]/(n*np.log(n)) for n in range(points) ],
        [ averages[5][n]/(n*np.log(np.log(n))) for n in range(points) ]
    ]

    for i in range(15):
    
        plt.figure()
        plt.scatter(xax, functions[i-12])
        plt.ylabel(labels[i])
        plt.xlabel("Urny")
        # plt.xscale('log') - ta linijka pozwala czasami na zmianę osi x na logarytm
    
        plt.gcf().set_size_inches(16, 9)
        plt.savefig("logs/"+str(i)+"fig2.svg")

def oldddd():
    plots()
    print("Gotowe")
    fig, axs = plt.subplots(3, len(os.listdir("images")))
    y = 0
    for folder in os.listdir("images"):
        x = 0
        for file in os.listdir(f"images/{folder}"):
            file = f"images/{folder}/{file}"

            image = cv.imread(file, cv.IMREAD_GRAYSCALE)
            image = cv.resize(image, (28, 28))
            image = image.astype('float32')
            image = image.reshape(1, 28, 28, 1)
            image = 255-image
            image /= 255

            pred = model.predict(image.reshape(1, 28, 28, 1), batch_size=1)

            axs[x,y].imshow(image.reshape(28, 28),cmap='Greys')
            axs[x,y].set_title(f"Predicted: {pred.argmax()}")
            

            # print(pred.argmax())
            x = x + 1
        y = y + 1

def avr(list):
    return sum(list) / len(list)

def genGraphs():
    filePrefixes = ["dijkstra", "radixheap", "dial"]
    graphs = [
        [ # Dijkstra
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # radixheap
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # dial
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ]
    ]

    for content in os.listdir("dijkstrares"):
        for i in range(3):
            f = open(filePrefixes[i]+"res/"+content, "r")
            V = 0
            E = 0
            C = 0
            data = []

            for line in f.readlines():
                if line[0] == 't':
                    data.append(float(line.split()[1]))
                if line[0] == 'g':
                    params = line.split()
                    V = int(params[1])
                    E = int(params[2])
                    C = int(params[4]) - int(params[3])
            f.close()
            graphs[i][0].append(np.average(data) / ( E * np.log(V) ))
            graphs[i][1].append(np.average(data) / ( E + V * C ))
            graphs[i][2].append(np.average(data) / ( E + V * np.log(V * C) ))
            print(( E * np.log(V) ),( E + V * C ),( E + V * np.log(V * C) ))

    fig, plots = plt.subplots(1, 3)
    # plt.figure()
    for algorithm in range(3):
        for formula in range(3):
            # plt.figure()
            # print(len(graphs[algorithm][formula]),len(graphs[algorithm][formula]))
            print(algorithm,formula,graphs[algorithm][formula])
            plots[algorithm].scatter(range(len(graphs[algorithm][formula])),graphs[algorithm][formula],label=str(algorithm))
            # plt.scatter(xax, functions[i-12])
            # plt.ylabel(labels[i])
            # plt.xlabel("Urny")
            plots[algorithm].set_title(filePrefixes[algorithm])
            # plt.xscale('log') - ta linijka pozwala czasami na zmianę osi x na logarytm
        
            # plt.gcf().set_size_inches(16, 9)
            # plt.savefig("logs/"+str(i)+"fig2.svg")
    # plt.figure()
    # plt.scatter(range(3),range(3))
    plt.savefig("graphs/algs.png")
        
def nineGraphs():
    filePrefixes = ["dijkstra", "radixheap", "dial"]
    graphs = [
        [ # Dijkstra
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # radixheap
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # dial
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ]
    ]

    for content in os.listdir("dijkstrares"):
        for i in range(3):
            f = open(filePrefixes[i]+"res/"+content, "r")
            V = 0
            E = 0
            C = 0
            data = []

            for line in f.readlines():
                if line[0] == 't':
                    data.append(float(line.split()[1]))
                if line[0] == 'g':
                    params = line.split()
                    V = int(params[1])
                    E = int(params[2])
                    C = int(params[4]) - int(params[3])
            f.close()
            graphs[i][0].append([d / ( E * np.log(V) ) for d in data])
            graphs[i][1].append([d / ( E + V * C ) for d in data])
            graphs[i][2].append([d / ( E + V * np.log(V * C)) for d in data])
            # print(( E * np.log(V) ),( E + V * C ),( E + V * np.log(V * C) ))

    fig, plots = plt.subplots(3, 3)
    fig.set_figheight(12)
    fig.set_figwidth(12)
    # plt.figure()
    formulas = ["E log(V)", "E + V*C", "E + V log(V*C) "]
    lines = [[[],[],[]],[[],[],[]],[[],[],[]]]
    for algorithm in range(3):
        for formula in range(3):
            # plt.figure()
            # print(len(graphs[algorithm][formula]),len(graphs[algorithm][formula]))
            # plt.scatter([], [], color = 'blue',label="cmp")
            # print(algorithm,formula,graphs[algorithm][formula])
            for n in range(len(graphs[algorithm][formula])):
                xax = [n for i in range(len(graphs[algorithm][formula][n]))]
                # xax = range(len(graphs[algorithm][formula][n]))
                plots[algorithm][formula].scatter(xax ,graphs[algorithm][formula][n], color="blue")
                lines[algorithm][formula].append(np.average(graphs[algorithm][formula][n]))
                # plots[algorithm][formula].scatter([n] ,[np.average(graphs[algorithm][formula][n])], color="red")
            # plt.scatter(xax, functions[i-12])
            # plt.ylabel(labels[i])
            # plt.xlabel("Urny")
            plots[algorithm][formula].plot(range(len(lines[algorithm][formula])),lines[algorithm][formula],color="red")
            plots[algorithm][formula].set_title(filePrefixes[algorithm] + " " + formulas[formula])
            
            # plt.xscale('log') - ta linijka pozwala czasami na zmianę osi x na logarytm
        
            # plt.gcf().set_size_inches(16, 9)
            # plt.savefig("logs/"+str(i)+"fig2.svg")
    # plt.figure()
    # plt.scatter(range(3),range(3))
    # plt.legend()
    plt.savefig("graphs/algs.png")

def nineGraphsNow():
    filePrefixes = ["dijkstra", "radixheap", "dial"]
    formulas = ["E log(V)", "E + V*C", "E + V log(V*C) "]
    graphs = [
        [ # Dijkstra
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # radixheap
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # dial
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ]
    ]
    fig, plots = plt.subplots(3, 3)
    fig.set_figheight(12)
    fig.set_figwidth(12)
    ccount = 0
    for content in os.listdir("dijkstrares"):
        for alg in range(3):
            f = open(filePrefixes[alg]+"res/"+content, "r")
            V = 0
            E = 0
            C = 0
            data = []

            for line in f.readlines():
                if line[0] == 't':
                    data.append(float(line.split()[1]))
                if line[0] == 'g':
                    params = line.split()
                    V = int(params[1])
                    E = int(params[2])
                    C = int(params[4]) - int(params[3])
            f.close()

            plots[alg,0].scatter([ccount for i in range(len(data))], [d / ( E * np.log(V) ) for d in data],color = "blue")
            plots[alg,1].scatter([ccount for i in range(len(data))], [d / ( E + V * C ) for d in data],color = "blue")
            plots[alg,2].scatter([ccount for i in range(len(data))], [d / ( E + V * np.log(V * C)) for d in data],color = "blue")

            # graphs[alg][0].append([d / ( E * np.log(V) ) for d in data])
            # graphs[alg][1].append([d / ( E + V * C ) for d in data])
            # graphs[alg][2].append([d / ( E + V * np.log(V * C)) for d in data])
            # print(( E * np.log(V) ),( E + V * C ),( E + V * np.log(V * C) ))
        ccount =ccount + 1
        
    for a in range(3):
        for f in range(3):
            plots[a][f].set_title(filePrefixes[a] + " " + formulas[f])

    plt.savefig("graphs/algs.png")
# nineGraphsNow()

def nineGraphsNowEr():
    algorithms = ["dijkstra", "radixheap", "dial"]
    formulas = ["E log(V)", "E + V*C", "E + V log(C) "]
    graphs = [
        [ # Dijkstra
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # radixheap
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ],[ # dial
            [], # E * log(V)
            [], # E + V*C
            []  # E + V*log(V*C)
        ]
    ]
    fig, plots = plt.subplots(3, 3)
    fig.set_figheight(24)
    fig.set_figwidth(24)

    ccount = 0
    for alg in range(len(algorithms)):
        fileI = 0
        for file in os.listdir(algorithms[alg]+"res"):
            f = open(algorithms[alg]+"res/"+file, "r")
            V = 0
            E = 0
            C = 0
            data = []
            for line in f.readlines():
                if line[0] == 't':
                    data.append(float(line.split()[1]))
                if line[0] == 'g':
                    params = line.split()
                    V = int(params[1])
                    E = int(params[2])
                    C = int(params[4]) - int(params[3])
            f.close()
            
            T = E
            if alg == 2:
                print(file)
                T = np.log(C)
            plots[alg, 0].scatter([T for i in range(len(data))], [d / ( E * np.log(V) ) for d in data],color = "blue")
            graphs[alg][0].append([T, np.average(data) / ( E * np.log(V) ) ])
            plots[alg, 1].scatter([T for i in range(len(data))], [d / ( E + V * C ) for d in data],color = "blue")
            graphs[alg][1].append([T, np.average(data) / ( E + V * C ) ])
            plots[alg, 2].scatter([T for i in range(len(data))], [d / ( E + V * np.log(C)) for d in data],color = "blue")
            graphs[alg][2].append([T, np.average(data) / ( E + V * np.log(C)) ])

            fileI = fileI + 1
        
    for a in range(3):
        for f in range(3):
            # graphs[a][f] = np.argsort( graphs[a][f] , 0 )
            xax = [graphs[a][f][i][0] for i in range(len(graphs[a][f]))]
            yax = [graphs[a][f][i][1] for i in range(len(graphs[a][f]))]
            plots[a][f].plot(xax,yax,color="red")
            plots[a][f].set_title(algorithms[a] + " " + formulas[f])

    plt.savefig("graphs/algs.png")

def compare():
    algorithms = ["dijkstra", "radixheap", "dial"]
    colors     = ["blue", "red", "green"]
    dat = [
        [ # Dijkstra
            
        ],[ # radixheap
        ],[ # dial
        ]
    ]
    fig = plt.figure()
    # fig, plots = plt.subplots(3, 3)
    # fig.set_figheight(24)
    # fig.set_figwidth(24)

    for alg in range(len(algorithms)):
        fileI = 0
        for file in os.listdir(algorithms[alg]+"resC"):
            f = open(algorithms[alg]+"resC/"+file, "r")
            V = 0
            E = 0
            C = 0
            data = []
            for line in f.readlines():
                if line[0] == 't':
                    data.append(float(line.split()[1]))
                if line[0] == 'g':
                    params = line.split()
                    V = int(params[1])
                    E = int(params[2])
                    C = int(params[4]) - int(params[3])
            f.close()
            dat[alg].append(data)
            
            # plt.scatter([fileI], [np.average(data)], color = colors[alg])

            fileI = fileI + 1
    plt.plot([np.average(dat[0][i]) for i in range(len(dat[0]))], label=algorithms[0])
    plt.plot([np.average(dat[1][i]) for i in range(len(dat[1]))], label=algorithms[1])
    plt.plot([np.average(dat[2][i]) for i in range(len(dat[2]))], label=algorithms[2])
    plt.legend()
    # plt.yscale('log')
    plt.savefig("graphs/compC.png")
    

compare()
# nineGraphsNowEr()

# genGraphs()
# plt.figure()
# plt.scatter(range(3),range(3))
# plt.show()
                
                




        