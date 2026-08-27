namespace sharppart.Models;
public class DataOutput : IData{
    public string text {get;set;}
    public int? Lenght {get;set;} = 0;
    public List<string> provide {get;set;}

    public DataOutput(string text, List<string> provide)
    {
        this.text = text;
        this.provide = provide;
        this.Lenght = text.Length;
    }

}