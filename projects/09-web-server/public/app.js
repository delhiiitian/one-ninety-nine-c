fetch("/api/time")
  .then((r) => r.json())
  .then((data) => { document.getElementById("time").textContent = data.utc; });
