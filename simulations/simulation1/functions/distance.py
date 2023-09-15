"""Some functions to compute distance between binary strings"""


def hamming_distance(s1: str, s2: str) -> float:
    """Compute the hamming distance between two binary strings.
    The hamming distance is the number of positions where the two strings differ.

    Args:
        s1 (str): The first binary string
        s2 (str): The second binary string

    Returns:
        float: The hamming distance between the two strings
    """
    if len(s1) != len(s2):
        raise ValueError("The two strings must have the same length")

    return sum(c1 != c2 for c1, c2 in zip(s1, s2))


def jaccard_similarity(s1: str, s2: str) -> float:
    """Compute the jaccard similarity between two binary strings.

    Args:
        s1 (str): The first binary string
        s2 (str): The second binary string

    Returns:
        float: The jaccard similarity between the two strings
    """
    if len(s1) != len(s2):
        raise ValueError("The two strings must have the same length")

    return sum(c1 == c2 for c1, c2 in zip(s1, s2)) / len(s1)


def study_distance(genotypes: list[str], distance_type: str) -> list:
    """Compute the distance between the genotypes of a study.

    Args:
        genotypes (list[str]): The list of genotypes
        distance_type (str): The type of distance to compute could be hamming or jaccard

    Returns:
        float: The distance between the genotypes
    """
    from itertools import combinations

    distances = []

    if distance_type.startswith("hamm"):
        func = hamming_distance
    elif distance_type.startswith("jacc"):
        func = jaccard_similarity
    else:
        raise ValueError("The distance type must be hamming or jaccard")

    for g1, g2 in combinations(genotypes, 2):
        distances.append(func(g1, g2))

    return distances


def shannon_diversity(genotypes: list[str]) -> float:
    """Computes the diversity of our population using the Shannon diversity index.

    Args:
        genotypes (list[str]): The genotypes of the population to study

    Returns:
        float: The diversity index
    """
    from collections import Counter
    from math import log

    # This creates a dictionary with the counts of each genotype
    counts = Counter(genotypes)

    total = len(genotypes)

    diversity = 0

    for count in counts.values():
        diversity -= (count / total) * log(count / total)

    return diversity
