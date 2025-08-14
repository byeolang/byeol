$(document).ready(function(e) {
  $('nav.toc header').click(function() {
    $('ul.toc__menu').toggle(500)
  })
  $('code.language-byeol').each(function(index) {
    $(this).attr("src", this.innerHTML)
  })
  hljs.highlightAll();
  $('code.hljs').hover(function() {
    var codeTag = $(this)[0]
    if($(this).find(".play_button").length) return
    $(this).append('<a class="play_button"><span/></a>')
    $(this).find('a.play_button').click(function() {
      redirectPlay(codeTag)
    })
  }, function() {
    $('.play_button').remove()
  })
})

function redirectPlay(codeTag) {
  var src = $(codeTag).attr("src")
  localStorage.setItem("src", src)
  window.open("/play", "_blank")
}