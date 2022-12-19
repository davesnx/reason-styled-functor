// Create a Styled functor

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
  let make = () => {
    React.createElement(ReactDOM.stringToComponent(Element.tag), ReactDOM.domProps(~className, ()))
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
      <Stack />
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
