from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    # Get lines
    lines_of_a = a.split('\n')
    lines_of_b = b.split('\n')

    # Use a set to store the line, beacuse
    # it's a better structure for string
    # and avoids duplicates values
    equal_lines = set()

    # Compares the lines
    for line_of_a in lines_of_a:
        if line_of_a in lines_of_b:
            equal_lines.add(line_of_a)

    return equal_lines


def sentences(a, b):
    """Return sentences in both a and b"""
    sentences_of_a = sent_tokenize(a)
    sentences_of_b = sent_tokenize(b)

    equal_sentences = set()

    # Compare sentences
    for sentence_of_a in sentences_of_a:
        if sentence_of_a in sentences_of_b:
            equal_sentences.add(sentence_of_a)

    # TODO
    return equal_sentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    substrings_a = get_substrings(a, n)
    substrings_b = get_substrings(b, n)

    equal_substrings = set()

    # Compare sentences
    for substring_a in substrings_a:
        if substring_a in substrings_b:
            equal_substrings.add(substring_a)
    return equal_substrings


def get_substrings(string, n):
    """Return a list of substrings of lenght for the given string"""
    substrings = set()

    for i in range(len(string) - n + 1):
        substrings.add(string[i:i+n])

    return [substring for substring in substrings]