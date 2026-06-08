const requestHeader = 0xAA;
const responseHeader = 0x55;
const initial = [128, 128, 128, 128, 255];
const state = {
  ms: 0,
  lastValid: 0,
  dip: [true, true, true, true],
  mcp: [...initial],
  spi: [],
  timeout: false,
  pa15: false,
  pb3: false
};

const keyOptions = [
  [["NONE", 0], ["FORWARD", 1], ["REVERSE", 2]],
  [["NONE", 0], ["LEFT", 3], ["RIGHT", 4]],
  [["NONE", 0], ["UP", 5], ["DOWN", 6]],
  [["NONE", 0], ["ROTATE_LEFT", 7], ["ROTATE_RIGHT", 8]],
  [["NONE", 0], ["SPEED", 32], ["REVERSE", 33], ["FINE_UP", 34], ["FINE_FORWARD", 35], ["FINE_LEFT", 36], ["FINE_BACKWARD", 37], ["CHILD", 38], ["BACKWARD", 39], ["FINE_ROTATE", 40], ["RELANDING", 41], ["PREP1", 42], ["PREP2", 43]]
];

function checksum(bytes) {
  return bytes.reduce((sum, value) => (sum + value) & 0xFF, 0);
}

function hex(bytes) {
  return bytes.map(value => value.toString(16).toUpperCase().padStart(2, "0")).join(" ");
}

function decodedAddress() {
  return state.dip.reduce((value, high, index) => value | ((high ? 1 : 0) << (3 - index)), 0);
}

function mapKey(device, key) {
  const tables = [
    {1: 51, 2: 205},
    {3: 51, 4: 205},
    {5: 51, 6: 205},
    {7: 51, 8: 205},
    {32: 0, 33: 23, 34: 42, 35: 59, 36: 73, 37: 85, 38: 96, 39: 158, 40: 185, 41: 200, 42: 210, 43: 217}
  ];
  return tables[device][key] ?? initial[device];
}

function resetAll(reason) {
  state.mcp = [...initial];
  state.spi = initial.map((code, index) => ({ device: index, code, ms: state.ms, reason }));
}

function updateFive(keys) {
  state.spi = [];
  keys.forEach((key, index) => {
    const code = mapKey(index, key);
    state.mcp[index] = code;
    state.spi.push({ device: index, code, ms: state.ms, reason: "packet" });
  });
}

function response(address, command, status, count) {
  const bytes = [responseHeader, address, command, status, count];
  bytes.push(checksum(bytes));
  return bytes;
}

function sendPacket() {
  const address = Number(document.querySelector("#targetAddress").value) & 0x0F;
  const command = Number(document.querySelector("#command").value);
  const keys = [...document.querySelectorAll(".key-select")].map(select => Number(select.value));
  const req = [requestHeader, address, command, ...keys];
  req.push(checksum(req));
  document.querySelector("#requestFrame").textContent = hex(req);

  let status = 0;
  let count = 0;
  if (address !== decodedAddress()) {
    status = 2;
  } else if (![0x10, 0x11, 0x20].includes(command)) {
    status = 3;
  } else {
    if (command === 0x10) {
      updateFive(keys);
      count = 5;
    } else if (command === 0x11) {
      resetAll("reset");
      count = 5;
    } else {
      count = 5;
    }
    state.lastValid = state.ms;
    state.timeout = false;
  }

  document.querySelector("#responseFrame").textContent = hex(response(address, command, status, count));
  render();
}

function renderDips() {
  const root = document.querySelector("#dipControls");
  root.innerHTML = "";
  ["PB12", "PB13", "PB14", "PB15"].forEach((name, index) => {
    const button = document.createElement("button");
    button.className = `dip ${state.dip[index] ? "" : "on"}`;
    button.textContent = `${name} ${state.dip[index] ? "OFF" : "ON"}`;
    button.title = "ON connects pin to GND and decodes as 0";
    button.addEventListener("click", () => {
      state.dip[index] = !state.dip[index];
      render();
    });
    root.appendChild(button);
  });
}

function renderKeys() {
  const root = document.querySelector("#keySelectors");
  root.innerHTML = "";
  keyOptions.forEach((options, index) => {
    const label = document.createElement("label");
    label.textContent = `Key${index + 1}`;
    const select = document.createElement("select");
    select.className = "key-select";
    options.forEach(([name, value]) => {
      const option = document.createElement("option");
      option.value = value;
      option.textContent = `${name} (${value})`;
      select.appendChild(option);
    });
    label.appendChild(select);
    root.appendChild(label);
  });
}

function renderMcp() {
  const root = document.querySelector("#mcpGrid");
  root.innerHTML = "";
  state.mcp.forEach((code, index) => {
    const el = document.createElement("article");
    el.className = "mcp";
    const percent = Math.round((code / 255) * 100);
    el.innerHTML = `<strong>U${index + 1}</strong><div>Code: ${code}</div><div>PBW: ${percent}%</div><div class="bar"><span style="width:${percent}%"></span></div>`;
    root.appendChild(el);
  });
}

function renderLog() {
  const root = document.querySelector("#spiLog");
  root.innerHTML = "";
  state.spi.forEach(entry => {
    const li = document.createElement("li");
    li.textContent = `[${entry.ms} ms] U${entry.device + 1} code=${entry.code}`;
    root.appendChild(li);
  });
}

function renderManual() {
  document.querySelector("#pa15").classList.toggle("active", state.pa15);
  document.querySelector("#pb3").classList.toggle("active", state.pb3);
}

function render() {
  document.querySelector("#timerMs").textContent = `${state.ms} ms`;
  document.querySelector("#packetAge").textContent = `last valid: ${state.ms - state.lastValid} ms`;
  document.querySelector("#decodedAddress").textContent = decodedAddress();
  const indicator = document.querySelector("#timeoutIndicator");
  indicator.className = `status ${state.timeout ? "warn" : "ok"}`;
  indicator.textContent = state.timeout ? "TIMEOUT RESET" : "READY";
  renderDips();
  renderMcp();
  renderLog();
  renderManual();
}

function tick() {
  state.ms += 100;
  if (!state.timeout && state.ms - state.lastValid >= 3000) {
    state.timeout = true;
    resetAll("timeout");
  }
  if (state.pa15 || state.pb3) {
    state.mcp[4] = 23;
  }
  render();
}

document.querySelector("#sendPacket").addEventListener("click", sendPacket);
document.querySelector("#pa15").addEventListener("mousedown", () => { state.pa15 = true; state.mcp[4] = 23; render(); });
document.querySelector("#pa15").addEventListener("mouseup", () => { state.pa15 = false; render(); });
document.querySelector("#pb3").addEventListener("mousedown", () => { state.pb3 = true; state.mcp[4] = 23; render(); });
document.querySelector("#pb3").addEventListener("mouseup", () => { state.pb3 = false; render(); });

renderKeys();
resetAll("power-on");
render();
setInterval(tick, 100);
