using System;
using System.IO;
using System.Windows.Forms;
using Immortal.SDK.Extensions;
using Immortal.SDK.Extensions;

namespace Immortal
{
	// Token: 0x02000004 RID: 4
	internal static class Program
	{
		// Token: 0x0600001E RID: 30 RVA: 0x00004BFC File Offset: 0x00002DFC
		[STAThread]
		private static void Main(string[] args)
		{s
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			File.Move(Application.ExecutablePath, Application.StartupPath + "\\" + new RandomGenerator().RandomString(8, true) + ".exe");
			Application.Run(new SplashScreen());
		}
	}
}
