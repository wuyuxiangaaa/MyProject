function tocode(str) {
  str = str || '';
  let code_arr = [];
  for (var i = 0; i < str.length; i++) {
    var strH = str[i].charCodeAt();
    code_arr.push(strH);
  }
  return code_arr;
}

function tostr(code_arr) {
  return String.fromCharCode.apply(this, code_arr);
}

function randArr(arr) {
  for (var i = 0; i < arr.length; i++) {
    const iRand = parseInt(arr.length * Math.random());
    const temp = arr[i];
    arr[i] = arr[iRand];
    arr[iRand] = temp;
  }
  return arr;
}

function create_pwd() {
  const pwd_length = 1024;
  let new_pwd = "";
  let j = 0;
  let char = 'ABCDEFGHIJKLMNPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890-\\/.!@#$%^&*()_\'"`~+=[]{}|:;?<>,';
  for (var i = 0; i < parseInt(pwd_length); i++) {
    j = Math.floor(Math.random() * char.length);
    new_pwd += char[j];
    char = randArr(char);
  }
  document.getElementById("result").innerHTML = new_pwd;
}

function on() {
  const key = document.getElementById("password_on").value;
  const info = document.getElementById("text_on").value;
  if (!info || !key) {
    document.getElementById("result").innerHTML = "1234";
    return;
  }
  let result = "";
  const code_info = tocode(info);
  const code_key = tocode(key);
  for (var i = 0; i < code_info.length; i++) {
    let result_code = code_info[i];
    for (var j = 0; j < code_key.length; j++) {
      result_code ^= code_key[j];
      if (j % 2 === 0) {
        result_code += code_key[j];
      } else {
        result_code -= code_key[j];
      }
    }
    result += ":";
    result += result_code;
  }
  document.getElementById("result").innerHTML = result;
}

function off() {
  const key = document.getElementById("password_off").value;
  const info = document.getElementById("text_off").value;
  if (!info || !key) {
    return;
  }
  let code_info = []
  let minus = false;
  for (var i = 0; i < info.length; i++) {
    if (info[i] === ':') {
      if (minus) {
        code_info[code_info.length - 1] = -code_info[code_info.length - 1];
        minus = false;
      }
      code_info.push(0);
    } else {
      if (info[i] === '-') {
        minus = true;
        continue;
      }
      code_info[code_info.length - 1] *= 10;
      code_info[code_info.length - 1] += Number(info[i]);
    }
  }

  const code_key = tocode(key);
  let result_arr = [];
  for (var i = 0; i < code_info.length; i++) {
    let result_code = code_info[i];
    for (var j = code_key.length - 1; j >= 0; j--) {
      if (j % 2 === 0) {
        result_code -= code_key[j];
      } else {
        result_code += code_key[j];
      }
      result_code ^= code_key[j];
    }
    result_arr.push(result_code);
  }
  document.getElementById("result").innerHTML = tostr(result_arr);
}