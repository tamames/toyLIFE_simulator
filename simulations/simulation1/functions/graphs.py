import os
import pandas as pd
from pathlib import Path
import matplotlib.pyplot as plt

print(os.getcwd())


def get_df(file: str) -> pd.DataFrame:
    """Reads a csv file and returns a pandas DataFrame with the data.
    we mainly have three types of files:
            food: information about the food pool
            geno: information about the genotypes at each generation
            popu: information about the population at each generation
    if the file doesn't match any of these cases, it will be read anyway.

    Args:
        file (str): The path to the file

    Returns:
        pd.DataFrame: the csv as a pandas DataFrame.
    """
    type_of_file = file[:4]

    data_folder = Path("simulations/simulation1/data")
    file_to_open = data_folder / file

    if type_of_file == "food":
        types = {
            "Max_1s": int,
            "Index": int,
            "Binary": str,
            "Total 1s": int,
            "Total 0s": int,
            "Total Elements": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif type_of_file == "geno":
        types = {
            "Max_1s": int,
            "Index": int,
            "Binary": str,
            "Total 1s": int,
            "Total 0s": int,
            "Total Elements": int,
            "Generation": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)

    elif type_of_file == "popu":
        types = {
            "Generation": int,
            "Average Energy Gain": float,
            "Max Energy Gain": int,
            "Min Energy Gain": int,
        }
        df = pd.read_csv(file_to_open, sep=",", header=0, dtype=types, index_col=False)
    else:
        print(f"The file {file} doesn't match any case. Reading anyway...")
        df = pd.read_csv(file_to_open, sep=",", header=0, index_col=False)

    return df


def graph_distribution_of_1(df: pd.DataFrame) -> None:
    df["1s Ratio"] = df["Total 1s"] / df["Total Elements"]

    # Create a bar plot
    plt.figure(figsize=(10, 6))
    plt.bar(df["Generation"], df["1s Ratio"], color="b", alpha=0.7)
    plt.xlabel("Generation")
    plt.ylabel("Ratio of 1s")
    plt.title("Distribution of 1s in Population")
    plt.grid(axis="y", linestyle="--", alpha=0.7)
    plt.show()


def graph_1s_and_0s(df: pd.DataFrame) -> None:
    df["1s Ratio"] = df["Total 1s"] / df["Total Elements"]

    df["0s Ratio"] = 1 - df["1s Ratio"]

    # Create an area plot
    plt.figure(figsize=(10, 6))
    plt.stackplot(
        df["Generation"],
        df["1s Ratio"],
        df["0s Ratio"],
        labels=["1s Ratio", "0s Ratio"],
        colors=["blue", "red"],
        alpha=0.7,
    )
    plt.xlabel("Generation")
    plt.ylabel("Ratio")
    plt.title("Distribution of 1s and 0s in Population")
    plt.legend()
    plt.grid(axis="y", linestyle="--", alpha=0.7)
    plt.show()


def main() -> None:
    df = get_df("genotype_info2.csv")
    graph_1s_and_0s(df)


if __name__ == "__main__":
    main()
