class Stats:

    def __init__(self, stats):
        self.energy = stats[0]
        self.laser = stats[1]
        self.regen = stats[2]
        self.max_clones = stats[3]

    def split(self, part):
        return [self.energy // part, self.laser // part, self.regen // part, self.max_clones // part]


class Ship:

    def __init__(self, ship):
        self.role = ship[0]
        self.id = ship[1]
        self.pos = ship[2]
        self.vel = ship[3]
        self.stats = Stats(ship[4])
        self.temp = ship[5]
        self.max_temp = ship[6]
        self.cheat = ship[7]
