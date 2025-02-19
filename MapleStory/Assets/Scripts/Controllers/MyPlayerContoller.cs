using UnityEngine;

public class MyPlayerContoller : PlayerController
{
    class Ability
    {
        public int STR;
        public int DEX;
        public int INT;
        public int LUK;
    }
    Ability _ability = new Ability();
    public int STR { get { return _ability.STR; } set { _ability.STR = value; } }
    public int DEX { get { return _ability.DEX; } set { _ability.DEX = value; } }
    public int INT { get { return _ability.INT; } set { _ability.INT = value; } }
    public int LUK { get { return _ability.LUK; } set { _ability.LUK = value; } }
    public int EXP { get; set; }
    public int HP { get; set; }
    public int MP { get; set; }
    protected override void Init()
    {
        base.Init();
    }
    protected override void UpdateController()
    {
        base.UpdateController();
    }
}
