// Create a Styled functor, this should be in a lib

module type ElementT = {
  let tag: string
  let styles: array(string)
};

let concat = styles => {
  switch styles {
  | [|one|] => one
  | styles => Array.fold_left((styles, item) => styles ++ " " ++ item, "", styles)->String.trim
  }
};

module Styled = (Element: ElementT) => {
  let className = concat(Element.styles);
  [@react.component]
  let make = (~children=React.null) => {
    let component = ReactDOM.stringToComponent(Element.tag);
    let props = ReactDOM.domProps(~className, ());
    React.createElementVariadic(component, props, [|children|])
  }
};

// Usage...
module Stack = Styled({
  let tag = "span"
  let styles = [|"flex", "justify-content"|]
});

module App = {
  // This sample forces an import of Belt.*, so that CI builds can ensure that
  // Melange has been installed correctly for JS bundlers to be able to find it.
  [@react.component]
  let make = () =>
    <div>
      <Stack>
        <p> {React.string("Hello")} </p>
        <p> {React.string("World")} </p>
      </Stack>
    </div>
};

ReactDOM.querySelector("#root")
->(
    fun
    | Some(root) => ReactDOM.render(<App />, root)
    | None =>
      Js.Console.error(
        "Failed to start React: couldn't find the #root element",
      )
  );
