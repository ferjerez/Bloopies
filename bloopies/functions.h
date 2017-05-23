// Check if board[x][y] tile matches with neighbors
bool match(int x,int y,byte target){
  return (board[x][y] == target || 
          board[x][y] == BOMB ||
          board[x][y] == HBOMB ||
          board[x][y] == VBOMB);
}
bool checkMatch(byte x, byte y) {
  byte pic = board[x][y];
  if (!flag[x][y]) {
    // Horizontal
    int xmin = x, xmax = x, ymin = y, ymax = y;

    for (int i = x + 1; i < MAXX && match(i,y,pic); i++) xmax = i; // right
    for (int i = x - 1; i >= 0 && match(i,y,pic); i--) xmin = i; // left
    if (xmax - xmin >= 2) {
      for (int i = xmin; i <= xmax; i++) {
        flag[i][y] = true;
      }
    }

    // Vertical
    for (int j = y + 1; j < MAXY && match(x,j,pic); j++) ymax = j; // down
    for (int j = y - 1; j >= 0 && match(x,j,pic); j--) ymin = j; // up
    if (ymax - ymin >= 2) {
      for (int j = ymin; j <= ymax; j++) {
        flag[x][j] = true;
      }
    }
  
    

  }

}

bool checkBoard() {
  for (byte x = 0; x < MAXX; x++) {
    for (byte y = 0; y < MAXY; y++) {
      checkMatch(x, y);
    }
  }
  // update flagcount
  flagCount = 0;
  for (byte x = 0; x < MAXX; x++) {
    for (byte y = 0; y < MAXY; y++) {
      if (flag[x][y]) flagCount++;
    }
  }
  // frame when the match occurs
  flagFrame = arduboy.frameCount;

  // scoring
  tmp_score+= mult_score*flagCount*multi[min(flagCount,5)];

  // Sound
  if(!title){ // Avoids sound when reset game
    if(flagCount>0){
      sound.tone(400+200*(mult_score-game_level), 100);
    }
  }

}

void refillBoard() {
  // from down to top

  for (byte x = 0; x < MAXX; x++) {
    for (int y = MAXY - 1; y >= 0; y--) {
      // empty flaged tiles
      if (flag[x][y]) {
        board[x][y] = EMPTY;
        falling = true;
      }

    }
  }

  for (byte x = 0; x < MAXX; x++) {
    for (int y = MAXY - 1; y >= 0; y--) {

      // drop new pieces
      while (board[x][y] == EMPTY) {
        for (int j = y; j > 0; j--) {
          board[x][j] = board[x][j - 1];
          offy[x][j] += 8;
        }
        // drop from above
        if(random(100)>101){ // TO DO.....DROP A BOMB
          board[x][0] = BOMB; // DROP A BOMB 
        }else{
          board[x][0] = floor(random(min(NUM_PICS, game_level + 3)));
        }
        
        offy[x][0] = max(8,offy[x][1]);
      }
      // unflag
      flag[x][y] = false;
    }
  }
}

void initBoard() {
  // init board
  for (byte x = 0; x < MAXX; x++) {
    for (byte y = 0; y < MAXY; y++) {
      board[x][y] = floor(random(min(NUM_PICS, game_level + 3)));
      flag[x][y] = false;
      offy[x][y] = 0;
    }
  }
}
