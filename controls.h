

class Controls {
  public:
  String name;
  Input* input;
  Output* output;
  String op = ">";
  Controls(String n,Input* in, Output* out):input{in},output{out},name{n}{};
};

