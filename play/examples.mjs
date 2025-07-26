export const examples = {
  'hello-world':
`# Hello, I'm Byeol language.
# Please edit your code freely and press button in the top right to run.
# Refer to the 'guide' section above for more syntax.
main() void
    print("hello world!")`,


  'bow-to-everyone':
`def person
    name := "default"
    bow() str
        print("hello. I'm " + name + "\\n")
        name

main() void
    mike := person()
    mike.name = "mike"

    last := for p in {mike, mike(), mike(mike)}
        p.bow()

    print("and second one was " + last[1])`
};