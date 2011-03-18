document.addEventListener('click', function folding(e) {
  var elem = e.target, is;
  do {
    if (/^a$/i.test(elem.nodeName)) return;
    is = elem.className || '';
  } while (!/\b(un)?folded /.test(is) && (elem = elem.parentNode));
  if (elem)
    elem.className = /unfolded /.test(is)
      ? is.replace('unfolded ', 'folded ')
      : is.replace('folded ', 'unfolded ');
}, false);
