#!/usr/bin/env python3
from collections import OrderedDict

class Node:
    def __init__(self, words):
        self.tokens = words
        self.deg = 0
        for w in words:
            if w.startswith(":"):
                self.deg += 1


def build_graph():
    g = OrderedDict()
    with open("./galaxy.txt", "r") as f:
        for i, line in enumerate(f):
            words = line.split()
            assert(words[1] == '=')
            key = words[0]
            words = words[2:]
            g[key] = Node(words)
    return g


def reduce_leaves(g):
    reduced = False
    for key, node in g.items():
        ntokens = []
        ndeg = 0
        for token in node.tokens:
            if token.startswith(":"):
                if g[token].deg == 0:
                    ntokens += g[token].tokens.copy()
                    reduced = True
                else:
                    ndeg += 1
                    ntokens += [token]
            else:
                ntokens += [token]
        node.tokens = ntokens
        node.deg = ndeg
    return reduced


def print_graph(g, used, node):
    print(node, '=', ' '.join(g[node].tokens))
    used.add(node)
    for token in g[node].tokens:
        if token.startswith(':') and token not in used:
            print_graph(g, used, token)


def main():
    g = build_graph()
    reduce_leaves(g)
    reduce_leaves(g)
    used = set()
    print_graph(g, used, "galaxy")


if __name__ == "__main__":
    main()
