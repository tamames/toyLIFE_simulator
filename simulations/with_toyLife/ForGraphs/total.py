from dataclasses import dataclass
from pathlib import Path

try:
    from ForGraphs.data_frames import get_total_df
except ModuleNotFoundError:
    from data_frames import get_total_df

import pandas as pd


@dataclass
class _DataMolecules:
    maximum_molecules: int
    table_molecules: str
    number_unique_molecules: int


@dataclass(kw_only=True)
class _GenotypeData:
    """this dataclass is the same as _FoodData in food.py."""

    genotype_len: int
    number_of_1: int
    mean_of_1: float
    percentage_of_1: float


@dataclass(kw_only=True)
class _TotalData:
    maximun_age: int
    median_age: float
    maximum_iterations: int
    number_of_children: int
    number_of_total_individuals: int
    number_of_total_genotypes: int
    data_prots: _DataMolecules
    data_mets: _DataMolecules
    data_dims: _DataMolecules
    genotype_data: _GenotypeData


def _total_statistics(df: pd.DataFrame) -> _TotalData:

    maximun_age = df["Age"].max()
    median_age = df["Age"].median()

    maximum_iterations = df["Iteration"].max()

    number_of_children = df[df["Parent"] > 0]["Parent"].nunique()
    number_of_total_individuals = df["ID"].nunique()
    number_of_total_genotypes = df["Genotype"].nunique()
    genotype_data = _process_genotype_data(df["Genotype"])

    data_prots = _process_molecules(df["Prots"])
    data_mets = _process_molecules(df["Mets"])
    data_dims = _process_molecules(df["Dims"])

    return _TotalData(
        maximun_age=maximun_age,
        median_age=median_age,
        maximum_iterations=maximum_iterations,
        number_of_children=number_of_children,
        number_of_total_individuals=number_of_total_individuals,
        number_of_total_genotypes=number_of_total_genotypes,
        data_prots=data_prots,
        data_mets=data_mets,
        data_dims=data_dims,
        genotype_data=genotype_data,
    )


def _process_genotype_data(genotype_column: pd.Series) -> _GenotypeData:
    """This function is almost a copy of _food_statistics in food.py.
    We do it this way to get the same data for food and genotypes so we can compare them.

    Args:
        genotype_column (pd.Series): the column with genotypes.

    Returns:
        _GenotypeData: a dataclass with the genotype data.
    """
    genotype_len = len(genotype_column[0])
    amount_of_food = len(genotype_column)

    total_slots = genotype_len * amount_of_food

    one_count: pd.Series = genotype_column.apply(lambda x: x.count("1"))

    number_of_1 = one_count.sum()
    mean_of_1 = one_count.mean()

    percentage_of_1 = number_of_1 / total_slots

    return _GenotypeData(
        genotype_len=genotype_len,
        number_of_1=number_of_1,
        mean_of_1=mean_of_1,
        percentage_of_1=percentage_of_1,
    )


def _process_molecules(molecule_column: pd.Series) -> _DataMolecules:
    not_empty = molecule_column.dropna()
    not_empty = not_empty.str.replace("{", "").str.replace("}", "").str.replace(" ", "")
    molecules = not_empty.str.split(",")

    df = pd.concat(
        [
            pd.DataFrame([agent], index=[idx])
            for idx, agent in zip(molecules.index, molecules)
        ],
        ignore_index=False,
    )
    # now we have a df where each row is an agent.
    # in each column we have the molecule and the amount.

    maximum_molecules = len(df.columns)

    total_molecules = pd.concat(
        [
            pd.DataFrame(
                [agent],
                columns=["molecule", "amount"],
            )
            for agent in molecules.explode().str.split(":")
        ]
    )

    total_molecules = total_molecules.astype(
        dtype={"molecule": "category", "amount": int}
    )

    total_molecules = total_molecules.groupby("molecule").sum()

    number_unique_molecules = total_molecules.shape[0]

    table_molecules = (
        total_molecules[total_molecules["amount"] > 1]
        .sort_values(by="amount", ascending=False)
        .to_markdown()
    )

    return _DataMolecules(maximum_molecules, table_molecules, number_unique_molecules)


def _write_total_data(data_folder_path: Path, total_data: _TotalData) -> None:
    """Writes more info into the Readme.md file.

    Args:
        data_folder_path (Path): the data folder we are looking at.
        total_data (_TotalData): the total dataclass.
    """

    with open(data_folder_path / "Readme.md", "a") as readme:
        readme.write("---  \n")
        readme.write("## Total Data  \n")
        readme.write("### General  \n")
        readme.write(f"maximun_age = {total_data.maximun_age}  \n")
        readme.write(f"median_age = {total_data.median_age}  \n")
        readme.write(f"maximum_iterations = {total_data.maximum_iterations}  \n")
        readme.write(f"number_of_children = {total_data.number_of_children}  \n")
        readme.write(
            f"number_of_total_individuals = {total_data.number_of_total_individuals}  \n"
        )
        readme.write(
            f"number_of_total_genotypes = {total_data.number_of_total_genotypes}  \n"
        )

        readme.write("### Prots  \n")
        readme.write(
            f"maximum_prots_at_once = {total_data.data_prots.maximum_molecules}  \n"
        )
        readme.write(
            f"number_unique_prots = {total_data.data_prots.number_unique_molecules}  \n\n"
        )
        readme.write(total_data.data_prots.table_molecules)

        readme.write("\n\n### Mets  \n")
        readme.write(
            f"maximum_mets_at_once = {total_data.data_mets.maximum_molecules}  \n"
        )
        readme.write(
            f"number_unique_mets = {total_data.data_mets.number_unique_molecules}  \n\n"
        )
        readme.write(total_data.data_mets.table_molecules)

        readme.write("\n\n### Dims  \n")
        readme.write(
            f"maximum_dims_at_once = {total_data.data_dims.maximum_molecules}  \n"
        )
        readme.write(
            f"number_unique_dims = {total_data.data_dims.number_unique_molecules}  \n\n"
        )
        readme.write(total_data.data_dims.table_molecules)

        readme.write("\n\n### Genotypes  \n")
        readme.write(f"genotype_len = {total_data.genotype_data.genotype_len}  \n")
        readme.write(
            f"total 1s in all genotypes = {total_data.genotype_data.number_of_1}  \n"
        )
        readme.write(
            f"mean 1s in all genotypes = {total_data.genotype_data.mean_of_1}  \n"
        )
        readme.write(
            f"percentage_of_1 = {total_data.genotype_data.percentage_of_1:.1%}  \n"
        )

        # TODO hacer un gráfico de los genotypos por iteracion, para ver cómo evolucionan los datos.


def main(data_folder_path: Path) -> None:
    df = get_total_df(data_folder_path)
    total_data = _total_statistics(df)
    _write_total_data(data_folder_path, total_data)


if __name__ == "__main__":
    ...
