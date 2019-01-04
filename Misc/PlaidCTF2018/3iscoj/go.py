oplist = []

class Variable():
    def __init__(self, index, flip):
        self.index = index
        self.flip = flip

    @classmethod
    def input(self):
        global oplist
        oplist
