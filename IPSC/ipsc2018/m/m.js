M.solvers = {};

M.solveCustomer = function(customer) {
  console.log("Intercepting", customer);
  if (!(customer.text in M.solvers)) {
    console.log("No solver found");
    return;
  }
  var answer = undefined;
  try {
    answer = M.solvers[customer.text](customer.var1, customer.var2);
  } catch (e) {
    console.log("Error running solver")
    console.log(e);
    return;
  }
  if (answer === undefined) {
    console.log("No answer found");
    return;
  }
  console.log("Answer is", answer);
  M.submitAnswer(customer.customer_id, answer);
  M.sendNotification("Solved customer!", {
    body: "Customer was " + JSON.stringify(customer) + " and answer was " + answer,
    tag: "ipsc2018m",
  });
};

M.oldAddCustomer = M.addCustomer;
M.addCustomer = function(customer) {
  M.oldAddCustomer(customer);
  M.solveCustomer(customer);
};

M.oldShowResultBubble = M.showResultBubble;
M.showResultBubble = function(text, optionalBubbleCoordinates) {
  M.oldShowResultBubble(text, optionalBubbleCoordinates);
  if (!text) return;
  if (text !== "Well done!") {
    M.sendNotification("BAD RESULT!", { body: text, tag: "ipsc2018m" })
  }
};


M.solvers["HELLO I WOULD LIKE TO ORDER FOOD ITEM var1."] = function(var1) {
  return var1;
};

M.solvers["OLLEH, I MA A BOTOR TAHT SKLAT SDRAWKCAB. EVIG EM DOOF var1 ESAELP."] = function(var1) {
  return var1.split("").reverse().join("");
};

M.solvers["GOOD DAY COUGH GIVE ME FOOD var1 COUGH COUGH COUGH. PARDON ME, THAT WAS A BUFFER OVERFLOW. I WANTED TO SAY THE FIRST 110 DIGITS OF THAT, PLEASE IGNORE THE REST."] = function(var1) {
  return var1.substring(0, 6);
};

M.solvers["GIVE ME ANY FOOD BETWEEN 1 AND 1000, INCLUSIVE. BUT NOT var1, I HATE THAT."] = function(var1) {
  if (var1 !== "1") {
    return "1";
  } else {
    return "1000";
  }
};

M.solvers["I WANT THE SQUARE ROOT OF var1."] = function(var1) {
  var int1 = parseInt(var1, 2);
  var res = Math.sqrt(int1).toString(2);
  return res;
}

M.solvers["I AM SO EXCITED! GIVE ME var1 AND YES, OF COURSE THAT IS A FACTORIAL!"] = function(var1) {
  var int1 = parseInt(var1, 2);
  if (var1 === int1.toString(2) + "!") {
    var res = 1;
    for (var i = 1; i <= int1; i++) {
      res *= i;
    }
    return res.toString(2);
  }
}

M.solvers["I HEARD var1 IS VERY TASTY, BUT I DO NOT UNDERSTAND THAT FOREIGN LANGUAGE. WHAT IS IT CALLED IN BINARY?"] = function(var1) {
  var res = eval(var1);
  console.log("Answer is " + res + "?");
  return res.toString(2);
}

M.solvers["I JUST WANT TO KNOW: IS var1 CORRECTLY NESTED? PLEASE ANSWER YES OR NO."] = function(var1) {
  var res = true;
  var stack = [];
  var inp = var1.split("");
  for (var i = 0; i < inp.length; i++) {
    var c = inp[i];
    if (c === '[' || c === '(' || c === '{') {
      stack.push(c);
    } else if (c === ']' || c === ')' || c === '}') {
      if (stack.length === 0) {
        res = false;
        break;
      }
      var o = stack.pop();
      var m = o + c;
      if (m === "[]" || m === "()" || m === "{}") {
        // we're good
      } else {
        res = false;
        break;
      }
    } else {
      console.log("HELP!");
      return;
    }
  }

  if (stack.length > 0) {
    res = false;
  }
  if (res) {
    return "YES";
  } else {
    return "NO";
  }
}

M.solvers["A STRANGE GAME. THE ONLY WINNING MOVE IS NOT TO PLAY.\nBUT WHO WON? ANSWER X OR O OR NEITHER.\n\nvar1"] = function(var1) {
  var board = var1.split('\n');
  for (var i = 0; i < 3; i++) {
    board[i] = board[i].split("");
  }

  var lines = [];
  for (var i = 0; i < 3; i++) {
    lines.push(board[i][0] + board[i][1] + board[i][2]);
    lines.push(board[0][i] + board[1][i] + board[2][i]);
  }
  lines.push(board[0][0] + board[1][1] + board[2][2]);
  lines.push(board[0][2] + board[1][1] + board[2][0]);

  if (lines.indexOf("XXX") !== -1) {
    return "X";
  } else if (lines.indexOf("OOO") !== -1) {
    return "O";
  } else {
    return "NEITHER";
  }
}

M.solvers["HOW MANY ISLANDS ARE ON THIS MAP?\n\nvar1"] = function(var1) {
  var board = var1.split('\n');
  for (let i = 0; i < board.length; i++) {
    board[i] = board[i].split('');
  }

  let dfs = function(x, y) {
    if (0 <= x && x < board.length && 0 <= y && y < board[x].length) {
      if (board[x][y] === '#') {
        board[x][y] = '.';
        dfs(x+1, y);
        dfs(x-1, y);
        dfs(x, y+1);
        dfs(x, y-1);
      }
    }
  }

  let res = 0;
  for (let i = 0; i < board.length; i++) {
    for (let j = 0; j < board[i].length; j++) {
      if (board[i][j] === '#') {
        res++;
        dfs(i, j);
      }
    }
  }

  return res.toString(2);
}

M.solvers["CAN YOU CHANGE THIS SHAPE TO THAT SHAPE JUST BY ROTATING OR FLIPPING IT? ANSWER YES OR NO.\n\nvar1\n\n\n\nvar2"] = function(var1, var2) {
  let to_board = function(v) {
    let board = v.split('\n');
    for (let i = 0; i < board.length; i++) {
      board[i] = board[i].split('');
    }
    return board;
  }
  let from_board = function(board) {
    for (let i = 0; i < board.length; i++) {
      board[i] = board[i].join('');
    }
    return board.join('\n');
  }

  let transpose = function(v) {
    let board = to_board(v);
    let result = [];
    for (let j = 0; j < board[0].length; j++) {
      let row = [];
      for (let i = 0; i < board.length; i++) {
        row.push(board[i][j]);
      }
      result.push(row);
    }
    return from_board(result);
  }

  let rev = function(v) {
    return v.split('\n').reverse().join('\n');
  }

  let rev2 = function(v) {
    return transpose(rev(transpose(v)));
  }

  for (let i = 0; i < 2; i++) {
    for (let j = 0; j < 2; j++) {
      for (let k = 0; k < 2; k++) {
        let a = i ? rev(var1) : var1;
        let b = j ? rev2(a) : a;
        let c = k ? transpose(b) : b;
        if (c === var2) return "YES";
      }
    }
  }

  return "NO";
}
