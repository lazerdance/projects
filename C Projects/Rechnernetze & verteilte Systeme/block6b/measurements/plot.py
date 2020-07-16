import matplotlib.pyplot as plt

FILEENDING = ".pdf"


def read_data(filenames: list) -> dict:
    data = {}
    for name in filenames:
        filedata = {"Offset": [], "Delay": [], "Root Dispersion": []}
        with open(name, "r") as file:
            for line in file:
                if "Hostname" in line:
                    continue
                splitted = line.split(";")
                filedata["Offset"].append(float(splitted[1]))
                filedata["Delay"].append(float(splitted[2]))
                filedata["Root Dispersion"].append(float(splitted[3]))
        data[name] = filedata
    return data


def build_plots():
    filenames = ["tu-berlin2.csv", "uni-paderborn2.csv", "uni-erlangen2.csv", "uni-bielefeld2.csv",
                 "tu-berlin.csv", "uni-paderborn.csv", "uni-erlangen.csv", "uni-bielefeld.csv",
                 "mazzanet-australia.csv"]
    data = read_data(filenames)
    x = range(100)

    # ## PLOT 1 - Offset/Delay ## #

    for i in ["Offset", "Delay"]:
        for j in range(4, 9):
            plt.plot(x, list(map(lambda x: x * (10**3), data[filenames[j]][i])),
                     label=filenames[j].split(".csv")[0])
        plt.legend(loc='best')
        # plt.title(i)
        plt.xlabel('Nummer des Aufrufs')
        plt.ylabel(i + " in Millisekunden")
        plt.savefig("../plots/" + i + "_all" + FILEENDING)
        plt.close()

    # ## PLOT 2 - Root Dispersion ## #

    for j in [6, 8]:
        plt.plot(x, list(map(lambda x: x * (10**6), data[filenames[j]]["Root Dispersion"])),
                 label=filenames[j].split(".csv")[0])

    ymin, ymax = plt.ylim()
    diff = ymax - ymin
    plt.ylim(ymin - diff * 0.5, ymax + diff * 0.5)

    plt.legend(loc='best')
    # plt.title("Root Dispersion")
    plt.xlabel('Nummer des Aufrufs')
    plt.ylabel("Root Dispersion in Mikrosekunden")
    plt.savefig("../plots/root_dispersion" + FILEENDING)
    plt.close()

    # ## PLOT 3 - Servers ## #

    for i in range(4, 9):
        plt.plot(x, list(map(lambda x: (x * (10**3)), data[filenames[i]]["Offset"])),
                 label="Offset")
        plt.plot(x, list(map(lambda x: x * (10**3), data[filenames[i]]["Delay"])),
                 label="Delay")
        plt.plot(x, list(map(lambda x: (x * (10**6)), data[filenames[i]]["Root Dispersion"])),
                 label="Root Dispersion")

        plt.legend(loc='best')
        # plt.title(filenames[i].split(".csv")[0])
        plt.xlabel('Nummer des Aufrufs')
        plt.ylabel("10e-3s (Offset/Delay) und 10e-6s (Disp.)")
        plt.savefig("../plots/server/" + filenames[i].split(".csv")[0] + FILEENDING)
        plt.close()

    # ## PLOT 4 - Different Measurements ## #

    for i in ["Offset", "Delay"]:
        for j in range(4):
            plt.plot(x, list(map(lambda x: x * (10**3), data[filenames[j + 4]][i])),
                     label=filenames[j + 4].split(".csv")[0])
            plt.plot(x, list(map(lambda x: x * (10**3), data[filenames[j]][i])),
                     label=filenames[j].split(".csv")[0])
            plt.legend(loc='best')
            # plt.title(i + " different measurements")
            plt.xlabel('Nummer des Aufrufs')
            plt.ylabel(i + " in Millisekunden")
            plt.savefig("../plots/diff/" + filenames[j + 4].split(".csv")[0] + "_compare_" + i + FILEENDING)
            plt.close()

    for j in range(4):
        plt.plot(x, list(map(lambda x: x * (10**6), data[filenames[j + 4]]["Root Dispersion"])),
                 label=filenames[j + 4].split(".csv")[0])
        plt.plot(x, list(map(lambda x: x * (10**6), data[filenames[j]]["Root Dispersion"])),
                 label=filenames[j].split(".csv")[0])

        ymin, ymax = plt.ylim()
        diff = ymax - ymin
        plt.ylim(ymin - diff * 0.5, ymax + diff * 0.5)

        plt.legend(loc='best')
        # plt.title("Root Dispersion different measurements")
        plt.xlabel('Nummer des Aufrufs')
        plt.ylabel("Root Dispersion in Mikrosekunden")
        plt.savefig("../plots/diff/" + filenames[j + 4].split(".csv")[0] + "_compare_rootDispersion" + FILEENDING)
        plt.close()

    return


if __name__ == "__main__":
    build_plots()
