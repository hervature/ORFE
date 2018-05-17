class TicTacToe(Game):
  current_state = None
  outcome = None
  players = {'P1':1, 'P2':0}
  num_players = 2
  rows, cols = 3, 3

  def __init__(self):
    # first plane is P1 moves, second plane is P2 moves
    self.current_state = [[[0 for k in range(self.rows)] for j in range(self.cols)] for i in range(self.num_players)]

  def generate_moves(self):
    # return 2D array of open squares, assume all are open. 0=closed, 1=open
    moves = [[1 for j in range(self.rows)] for i in range(self.cols)]

    for plane in self.current_state:
      for x in range(self.rows):
        for y in range(self.cols):
          if plane[y][x] == 1:
            moves[y][x] = 0

    return moves

  def transition(self, action):
    # use the proper player's plane to mark move
    # action is a tuple of the coordinate (x,y) to play
    plane_to_use = 0 if self.players['P1'] == 1 else 1
    x, y = action[0], action[1]

    plane = self.current_state[plane_to_use]
    plane[y][x] = 1

    # check winning condition, rows
    for x in range(self.rows):
      count = 0
      for y in range(self.cols):
        count += plane[y][x]

      if count == 3:
        self.outcome = 'P1' if self.players['P1'] == 1 else 'P2'
        return

    # check winning condition, columns
    for y in range(self.cols):
      count = 0
      for x in range(self.rows):
        count += plane[y][x]

      if count == 3:
        self.outcome = 'P1' if self.players['P1'] == 1 else 'P2'
        return

    # check winning condition, diagonals
    count = 0
    for x in range(self.rows):
      count += plane[x][x]

    if count == 3:
      self.outcome = 'P1' if self.players['P1'] == 1 else 'P2'
      return

    count = 0
    for x in range(self.rows):
      count += plane[x][-1-x]

    if count == 3:
      self.outcome = 'P1' if self.players['P1'] == 1 else 'P2'
      return

    self.players['P1'] = not self.players['P1']
    self.players['P2'] = not self.players['P2']
